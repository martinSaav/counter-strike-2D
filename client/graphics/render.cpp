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
        hud(sdlRenderer, configuration, assetsPath),
        map(sdlRenderer, configuration, assetsPath),
        player(sdlRenderer, configuration, assetsPath) {}

void Render::renderFrame(std::optional<GameStateUpdate> message) {

    std::list<PlayerInfo> players = message->get_players();

    float zoom = configuration.zoom;

    int worldWidth = map.getWidth();
    int worldHeight = map.getHeight();

    int camWidth = configuration.widthWindow / zoom;
    int camHeight = configuration.heightWindow / zoom;

    // Calcula la posición del jugador
    for (auto& playerInfo: players) {
        if (playerInfo.get_user_name() == namePlayer) {
            myPlayer = &playerInfo;
        }
    }

    SDL_Rect& camera = configuration.camera;

    camera.x = myPlayer->get_pos_x() - camWidth / 1.5;
    camera.y = myPlayer->get_pos_y() - camHeight / 1.5;

    // Limitar bordes
    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > worldWidth - camWidth)
        camera.x = worldWidth - camWidth;
    if (camera.y > worldHeight - camHeight)
        camera.y = worldHeight - camHeight;

    sdlRenderer->Clear();

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

    map.draw();  // Dibujo el mapa

    // Obtengo mi angulo
    SDL_GetMouseState(&mouseX, &mouseY);
    int mouse_map_x = int(mouseX / zoom + camera.x);
    int mouse_map_y = int(mouseY / zoom + camera.y);

    // Dibuja jugadores con zoom
    int posPlayerX = 0;
    int posPlayerY = 0;
    int mousePlayerX = 0;
    int mousePlayerY = 0;

    // visionAngle: debe ser el ángulo real hacia donde está mirando el jugador (sin correcciones de
    // sprite ni nada).
    myAngle =
            getAnglePlayer(myPlayer->get_pos_x(), myPlayer->get_pos_y(), mouse_map_x, mouse_map_y);
    float visionAngle = myAngle;
    if (visionAngle < 0)
        visionAngle += 360.0;

    // Reset bomb
    if (message->get_round_time() == 0) {
        map.desactivateBomb();
    }

    bool is_bomb_planted = message->is_bomb_planted();
    // Draw bomb
    if (is_bomb_planted && !message->is_round_ended()) {
        int bomb_x = message->get_bomb_x();
        int bomb_y = message->get_bomb_y();

        if (pointInVision(myPlayer->get_pos_x(), myPlayer->get_pos_y(), visionAngle, bomb_x,
                          bomb_y)) {
            map.drawBomb(bomb_x, bomb_y);
        }

        map.activateBomb();
    }

    if (is_bomb_planted && message->is_round_ended() && message->get_bomb_timer() == 0 &&
        message->get_round_winner() == Team::Terrorists) {
        if (map.isBombActivated()) {
            map.exploitBomb();
        }
    }

    for (const auto& stain: bloodStains) {
        player.drawBlood(stain.x, stain.y);
    }

    for (auto const& playerInfo: players) {

        std::string name = playerInfo.get_user_name();
        int currentHealth = playerInfo.get_health();

        lastHealths.try_emplace(name, currentHealth);

        if (currentHealth < lastHealths[name]) {
            bloodStains.push_back({playerInfo.get_pos_x(), playerInfo.get_pos_y()});
        }

        lastHealths[name] = currentHealth;

        if (playerInfo.get_health() == 0) {
            player.drawPlayerDeath(playerInfo.get_pos_x(), playerInfo.get_pos_y());
            continue;
        }

        if (playerInfo.get_user_name() == namePlayer) {
            player.drawPlayer(playerInfo, myAngle);
            continue;
        }

        posPlayerX = playerInfo.get_pos_x();
        posPlayerY = playerInfo.get_pos_y();
        mousePlayerX = playerInfo.get_pos_shoot_x();
        mousePlayerY = playerInfo.get_pos_shoot_y();
        anglePlayer = getAnglePlayer(posPlayerX, posPlayerY, mousePlayerX, mousePlayerY);

        if (pointInVision(myPlayer->get_pos_x(), myPlayer->get_pos_y(), visionAngle,
                          playerInfo.get_pos_x(), playerInfo.get_pos_y())) {
            player.drawPlayer(playerInfo, anglePlayer);
        }
    }

    // El servidor manda cuanto le queda a la etapa actual (compra, ronda o bomba), asi el
    // reloj siempre cuenta hacia atras.
    int time = static_cast<int>(std::ceil(message->get_time_left()));
    int round = message->get_round();
    bool isBuyStage = message->get_stage() == RoundStage::Buy;

    SDL_Rect mouse = {mouseX, mouseY, 40, 40};
    hud.draw(mouse, time, round, *myPlayer, is_bomb_planted, isBuyStage);

    if (message->is_round_ended()) {
        Team team = message->get_round_winner();
        map.drawEndRound(team, message->is_bomb_planted());

        bloodStains.clear();
        lastHealths.clear();
    }
    if (!isBuyStage && myPlayer->get_status() == Status::Alive) {
        map.drawCampField(myAngle, myPlayer->get_pos_x(), myPlayer->get_pos_y());
    } else if (isBuyStage) {
        map.drawShop();
    }

    map.drawExplosion();

    std::list<DroppedWeapon> dropped_weapons = message->get_dropped_weapons();
    for (auto weapon: dropped_weapons) {
        if (pointInVision(myPlayer->get_pos_x(), myPlayer->get_pos_y(), visionAngle,
                          weapon.get_pos_x(), weapon.get_pos_y())) {
            hud.drawWeaponDroped(weapon.get_weapon(), weapon.get_pos_x(), weapon.get_pos_y());
        }
    }

    int currentHealth = myPlayer->get_health();
    if (lastHealth != -1 && currentHealth < lastHealth) {
        damageFlashTimer = 10;
    }
    lastHealth = currentHealth;

    if (damageFlashTimer > 0) {
        SDL_Rect screen = {0, 0, configuration.widthWindow, configuration.heightWindow};
        sdlRenderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        sdlRenderer->SetDrawColor(255, 0, 0, 120);  // Rojo semi-transparente
        sdlRenderer->FillRect(screen);
        damageFlashTimer--;
    }

    sdlRenderer->Present();
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
    float radius = 60.0f;
    float dx = puntoX - playerX;
    float dy = puntoY - playerY;
    float distancia = std::sqrt(dx * dx + dy * dy);

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
