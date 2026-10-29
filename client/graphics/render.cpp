#include "render.h"

#include <algorithm>
#include <cmath>
#include <list>

using SDL2pp::Rect;

Render::Render(Renderer* renderer, Protocol& protocol, std::string& namePlayer,
               Configuration& configuration, std::string& assetsPath):
        sdlRenderer(renderer),
        protocol(protocol),
        namePlayer(namePlayer),
        configuration(configuration),
        assetsPath(assetsPath),
        textures(sdlRenderer),
        sounds(assetsPath),
        hud(sdlRenderer, configuration, textures, sounds, assetsPath),
        map(sdlRenderer, configuration, textures, sounds, assetsPath),
        player(sdlRenderer, configuration, textures, sounds, assetsPath) {}

void Render::renderFrame(std::optional<GameStateUpdate> message) {
    // Un frame se dibuja por capas, en este orden (lo que va despues queda encima): mapa, bomba,
    // jugadores, HUD, overlays del mundo (campo de vision o tienda, explosion, armas tiradas) y
    // por ultimo el feedback (barra de progreso, ayuda, flash de dano).
    FrameContext frame{*message, message->get_players()};
    for (const auto& playerInfo: frame.players) {
        if (playerInfo.get_user_name() == namePlayer) {
            frame.me = &playerInfo;
        }
    }
    if (!frame.me) {
        return;  // todavia no llego un estado que nos incluya
    }
    frame.isBuyStage = frame.state.get_stage() == RoundStage::Buy;

    updateCamera(*frame.me);
    sdlRenderer->Clear();
    map.draw();
    updateAim(frame);
    drawBomb(frame);
    drawPlayers(frame);
    drawHudLayer(frame);
    drawWorldOverlays(frame);
    drawFeedback(frame);
    sdlRenderer->Present();
}

void Render::updateCamera(const PlayerInfo& me) {
    // La camara sigue al jugador sin salirse del mapa; si hubo una explosion, tiembla un rato
    SDL_Rect& camera = configuration.camera;
    const int camWidth = configuration.widthWindow / configuration.zoom;
    const int camHeight = configuration.heightWindow / configuration.zoom;
    camera.x = me.get_pos_x() - camWidth / 1.5;
    camera.y = me.get_pos_y() - camHeight / 1.5;
    camera.x = std::min(std::max(camera.x, 0), map.getWidth() - camWidth);
    camera.y = std::min(std::max(camera.y, 0), map.getHeight() - camHeight);

    if (map.isShaking()) {
        int elapsed = SDL_GetTicks() - map.getShakeStartTime();
        if (elapsed < map.getShakeDuration()) {
            int offset_x = (rand() % (2 * map.getShakeMagnitude() + 1)) - map.getShakeMagnitude();
            int offset_y = (rand() % (2 * map.getShakeMagnitude() + 1)) - map.getShakeMagnitude();
            camera.x += offset_x;
            camera.y += offset_y;
        } else {
            map.stopShake();
        }
    }
}

void Render::updateAim(FrameContext& frame) {
    // Angulo real hacia el mouse (sin correcciones de sprite): define el campo de vision
    SDL_GetMouseState(&mouseX, &mouseY);
    const int mouse_map_x = int(mouseX / configuration.zoom + configuration.camera.x);
    const int mouse_map_y = int(mouseY / configuration.zoom + configuration.camera.y);
    frame.myAngle =
            getAnglePlayer(frame.me->get_pos_x(), frame.me->get_pos_y(), mouse_map_x, mouse_map_y);
    frame.visionAngle = frame.myAngle < 0 ? frame.myAngle + 360.0 : frame.myAngle;
}

bool Render::isVisible(const FrameContext& frame, int x, int y) {
    return pointInVision(frame.me->get_pos_x(), frame.me->get_pos_y(), frame.visionAngle, x, y);
}

void Render::drawBomb(const FrameContext& frame) {
    const GameStateUpdate& state = frame.state;
    if (state.get_round_time() == 0) {
        map.desactivateBomb();  // empezo una ronda nueva
    }
    const bool planted = state.is_bomb_planted();
    if (planted && !state.is_round_ended()) {
        if (isVisible(frame, state.get_bomb_x(), state.get_bomb_y())) {
            map.drawBomb(state.get_bomb_x(), state.get_bomb_y());
        }
        map.activateBomb();
    }
    if (planted && state.is_round_ended() && state.get_bomb_timer() == 0 &&
        state.get_round_winner() == Team::Terrorists && map.isBombActivated()) {
        map.exploitBomb();
    }
}

void Render::drawPlayers(const FrameContext& frame) {
    for (const auto& stain: bloodStains) {
        player.drawBlood(stain.x, stain.y);
    }
    const Uint32 now = SDL_GetTicks();
    for (const auto& playerInfo: frame.players) {
        playPlayerSounds(frame, playerInfo, now);

        if (playerInfo.get_health() == 0) {
            player.drawPlayerDeath(playerInfo.get_pos_x(), playerInfo.get_pos_y());
        } else if (&playerInfo == frame.me) {
            player.drawPlayer(playerInfo, frame.myAngle);
        } else if (isVisible(frame, playerInfo.get_pos_x(), playerInfo.get_pos_y())) {
            // Los demas miran hacia su mira, que manda el servidor
            const double angle =
                    getAnglePlayer(playerInfo.get_pos_x(), playerInfo.get_pos_y(),
                                   playerInfo.get_pos_shoot_x(), playerInfo.get_pos_shoot_y());
            player.drawPlayer(playerInfo, angle);
        }
    }
}

void Render::playPlayerSounds(const FrameContext& frame, const PlayerInfo& playerInfo, Uint32 now) {
    // Dano, muerte, pasos y disparos de cada jugador, mas fuertes cuanto mas cerca de mi
    const std::string& name = playerInfo.get_user_name();
    const int currentHealth = playerInfo.get_health();
    const float dx = playerInfo.get_pos_x() - frame.me->get_pos_x();
    const float dy = playerInfo.get_pos_y() - frame.me->get_pos_y();
    const float distanceToMe = std::sqrt(dx * dx + dy * dy);

    lastHealths.try_emplace(name, currentHealth);
    if (currentHealth < lastHealths[name]) {
        bloodStains.push_back({playerInfo.get_pos_x(), playerInfo.get_pos_y()});
        player.playSound(currentHealth == 0 ? DEATH : HIT, currentHealth == 0 ? 110 : 90,
                         distanceToMe);
    }
    lastHealths[name] = currentHealth;

    // Pasos: un sonido cada tanto mientras el jugador se mueve
    const std::pair<int, int> position(playerInfo.get_pos_x(), playerInfo.get_pos_y());
    const auto lastPosition = lastPositions.find(name);
    if (lastPosition != lastPositions.end() && lastPosition->second != position &&
        currentHealth > 0 && now - lastStepTicks[name] > 350) {
        player.playSound(STEP, 110, distanceToMe);
        lastStepTicks[name] = now;
    }
    lastPositions[name] = position;

    if (currentHealth > 0) {
        player.playShotSound(playerInfo, distanceToMe);
    }
}

void Render::drawHudLayer(const FrameContext& frame) {
    // El servidor manda cuanto le queda a la etapa actual (compra, ronda o bomba), asi el reloj
    // siempre cuenta hacia atras.
    int time = static_cast<int>(std::ceil(frame.state.get_time_left()));
    int round = frame.state.get_round();
    bool planted = frame.state.is_bomb_planted();
    SDL_Rect mouse = {mouseX, mouseY, 40, 40};
    PlayerInfo me = *frame.me;
    hud.draw(mouse, time, round, me, planted, frame.isBuyStage);

    if (frame.state.is_round_ended()) {
        map.drawEndRound(frame.state.get_round_winner(), planted);
        bloodStains.clear();
        lastHealths.clear();
    }
}

void Render::drawWorldOverlays(const FrameContext& frame) {
    if (!frame.isBuyStage && frame.me->get_status() == Status::Alive) {
        // drawCampField espera el angulo del sprite (90 grados mas que el de la mira)
        map.drawCampField(frame.myAngle + 90, frame.me->get_pos_x(), frame.me->get_pos_y());
    } else if (frame.isBuyStage) {
        map.drawShop();
    }
    map.drawExplosion();
    for (const auto& weapon: frame.state.get_dropped_weapons()) {
        if (isVisible(frame, weapon.get_pos_x(), weapon.get_pos_y())) {
            hud.drawWeaponDroped(weapon.get_weapon(), weapon.get_pos_x(), weapon.get_pos_y());
        }
    }
}

void Render::drawFeedback(const FrameContext& frame) {
    const PlayerInfo& me = *frame.me;
    const int currentHealth = me.get_health();
    if (lastHealth != -1 && currentHealth < lastHealth) {
        damageFlashTimer = 10;
    }
    lastHealth = currentHealth;

    const int taskProgress = me.get_task_progress();
    if (taskProgress > 0 && me.get_status() == Status::Alive) {
        const bool planting = me.get_skin()[0] != 'C';
        hud.drawTaskProgress(taskProgress, planting);
        if (!planting && lastTaskProgress == 0) {
            player.playSound(DEFUSING, 100, 0);
        }
    }
    lastTaskProgress = taskProgress;

    playStateSounds(frame.state, me, frame.isBuyStage);

    // La ayuda aparece sola en la primera compra; despues solo si la abren con H
    if (!frame.isBuyStage && !configuration.helpToggled) {
        configuration.helpVisible = false;
    }
    if (configuration.helpVisible) {
        hud.drawHelp();
    }

    if (damageFlashTimer > 0) {
        SDL_Rect screen = {0, 0, configuration.widthWindow, configuration.heightWindow};
        sdlRenderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        sdlRenderer->SetDrawColor(255, 0, 0, 120);  // rojo semitransparente
        sdlRenderer->FillRect(screen);
        damageFlashTimer--;
    }
}

double Render::getAnglePlayer(int playerX, int playerY, int mousex, int mousey) {

    // Calcular el ángulo entre el jugador y el mouse
    int dx = mousex - playerX;
    int dy = mousey - playerY;

    double angleRadians = atan2(dy, dx);  // da un ángulo en radianes entre -π y π
    double angleDegrees = angleRadians * 180 / M_PI;

    return angleDegrees;  // Ajustar para que apunte hacia la dirección del mouse
}

bool Render::pointInVision(int playerX, int playerY, float visionAngleDeg, int puntoX, int puntoY) {
    float fovDeg = configuration.visionCone;
    float radius = configuration.visionRadius();
    // Lo que esta pegado al jugador se ve siempre, aunque quede fuera del cono
    const float nearRadius = 6.0f;
    float dx = puntoX - playerX;
    float dy = puntoY - playerY;
    float distancia = std::sqrt(dx * dx + dy * dy);

    if (distancia <= nearRadius)
        return true;

    // Esta dentro del radio
    if (distancia > radius)
        return false;

    // angulo hacia el punto
    float angleToPoint = std::atan2(dy, dx) * 180.0f / M_PI;

    // Normalizar ángulos a [0, 360)
    visionAngleDeg = normalizeAngle(visionAngleDeg);
    angleToPoint = normalizeAngle(angleToPoint);

    // normalizá ángulos a [0, 360)
    if (angleToPoint < 0)
        angleToPoint += 360;
    if (visionAngleDeg < 0)
        visionAngleDeg += 360;

    float delta = std::fabs(angleToPoint - visionAngleDeg);

    // Manejar borde entre 0 y 360
    if (delta > 180.0f)
        delta = 360.0f - delta;

    return delta <= (fovDeg / 2.0f);
}

float Render::normalizeAngle(float angle) {
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return angle;
}

void Render::clearScreen() {
    sdlRenderer->SetDrawColor(0, 0, 0, 255);
    sdlRenderer->Clear();
}

void Render::playStateSounds(const GameStateUpdate& message, const PlayerInfo& me,
                             bool isBuyStage) {
    const Uint32 now = SDL_GetTicks();

    // Bomba plantada: un pitido que se acelera en los ultimos 10 segundos, mas fuerte cuanto
    // mas cerca este la bomba
    if (message.is_bomb_planted() && !message.is_round_ended()) {
        const float timeLeft = message.get_time_left();
        const Uint32 interval = static_cast<Uint32>(std::clamp(timeLeft * 100.0f, 150.0f, 1000.0f));
        if (now - lastBombBeep >= interval) {
            const float dx = message.get_bomb_x() - me.get_pos_x();
            const float dy = message.get_bomb_y() - me.get_pos_y();
            player.playSound(C4_BEEP, 100, std::sqrt(dx * dx + dy * dy));
            lastBombBeep = now;
        }
    }

    const bool alive = me.get_status() == Status::Alive;

    // Gatillo sin balas: el servidor no dispara, asi que el aviso lo da el cliente
    const Weapon active = me.get_active_weapon();
    const bool isGun = active == Weapon::Glock || active == Weapon::AK47 || active == Weapon::M3 ||
                       active == Weapon::AWP;
    const bool clicking = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);
    if (alive && !isBuyStage && clicking && isGun && me.get_active_weapon_ammo() == 0 &&
        now - lastEmptyClick > 350) {
        player.playSound(EMPTY_CLICK, 80, 0);
        lastEmptyClick = now;
    }

    // Levantar (E) o tirar (G) la principal o la bomba. Comprar tiene su propio sonido, y al
    // morir o plantar tambien se pierden armas sin que sea "tirarlas".
    const Weapon primary = me.get_primary_weapon();
    const bool hasBomb = me.get_bomb() == Weapon::Bomb;
    if (alive && wasAlive && me.get_action() != Action::BuyingWeapon) {
        const bool gotWeapon =
                (primary != lastPrimary && primary != Weapon::None) || (hasBomb && !lastHadBomb);
        const bool lostWeapon = (primary == Weapon::None && lastPrimary != Weapon::None) ||
                                (!hasBomb && lastHadBomb && !message.is_bomb_planted());
        if (gotWeapon) {
            player.playSound(PICKUP, 90, 0);
        } else if (lostWeapon) {
            player.playSound(DROP, 90, 0);
        }
    }
    lastPrimary = primary;
    lastHadBomb = hasBomb;
    wasAlive = alive;
}
