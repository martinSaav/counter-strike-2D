#include "render.h"

#include <algorithm>
#include <cmath>
#include <list>

using SDL2pp::Rect;

Render::Render(Renderer* renderer, Protocol& protocolo, std::string& namePlayer,
               Configuracion& configuracion, std::string& assetsPath):
        sdlRenderer(renderer),
        protocolo(protocolo),
        namePlayer(namePlayer),
        configuracion(configuracion),
        assetsPath(assetsPath),
        hud(sdlRenderer, configuracion, assetsPath),
        mapa(sdlRenderer, configuracion, assetsPath),
        player(sdlRenderer, configuracion, assetsPath) {}

void Render::renderFrame(std::optional<GameStateUpdate> mensaje) {

    std::list<PlayerInfo> jugadores = mensaje->get_players();

    float zoom = configuracion.zoom;

    int worldWidth = mapa.getWidth();
    int worldHeight = mapa.getHeight();

    int camWidth = configuracion.widthWindow / zoom;
    int camHeight = configuracion.heightWindow / zoom;

    // Calcula la posición del jugador
    for (auto& jugador: jugadores) {
        if (jugador.get_user_name() == namePlayer) {
            myPlayer = &jugador;
        }
    }

    SDL_Rect& camera = configuracion.camera;

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

    if (mapa.isShaking()) {
        int elapsed = SDL_GetTicks() - mapa.getShakeStartTime();
        if (elapsed < mapa.getShakeDuration()) {
            int offset_x = (rand() % (2 * mapa.getShakeMagnitude() + 1)) - mapa.getShakeMagnitude();
            int offset_y = (rand() % (2 * mapa.getShakeMagnitude() + 1)) - mapa.getShakeMagnitude();
            camera.x += offset_x;
            camera.y += offset_y;
        } else {
            mapa.stopShake();
        }
    }

    mapa.draw();  // Dibujo el mapa

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
    if (mensaje->get_round_time() == 0) {
        mapa.desactivateBomb();
    }

    bool is_bomb_planted = mensaje->is_bomb_planted();
    // Draw bomb
    if (is_bomb_planted && !mensaje->is_round_ended()) {
        int bomb_x = mensaje->get_bomb_x();
        int bomb_y = mensaje->get_bomb_y();

        if (puntoEnVision(myPlayer->get_pos_x(), myPlayer->get_pos_y(), visionAngle, bomb_x,
                          bomb_y)) {
            mapa.drawBomb(bomb_x, bomb_y);
        }

        mapa.activateBomb();
    }

    if (is_bomb_planted && mensaje->is_round_ended() && mensaje->get_bomb_timer() == 0 &&
        mensaje->get_round_winner() == Team::Terrorists) {
        if (mapa.isBombActivated()) {
            mapa.exploitBomb();
        }
    }

    for (const auto& stain: bloodStains) {
        player.drawBlood(stain.x, stain.y);
    }

    for (auto const& jugador: jugadores) {

        std::string nombre = jugador.get_user_name();
        int vida_actual = jugador.get_health();

        lastHealths.try_emplace(nombre, vida_actual);

        if (vida_actual < lastHealths[nombre]) {
            bloodStains.push_back({jugador.get_pos_x(), jugador.get_pos_y()});
        }

        lastHealths[nombre] = vida_actual;

        if (jugador.get_health() == 0) {
            player.drawPlayerDeath(jugador.get_pos_x(), jugador.get_pos_y());
            continue;
        }

        if (jugador.get_user_name() == namePlayer) {
            player.drawPlayer(jugador, myAngle);
            continue;
        }

        posPlayerX = jugador.get_pos_x();
        posPlayerY = jugador.get_pos_y();
        mousePlayerX = jugador.get_pos_shoot_x();
        mousePlayerY = jugador.get_pos_shoot_y();
        anglePlayer = getAnglePlayer(posPlayerX, posPlayerY, mousePlayerX, mousePlayerY);

        if (puntoEnVision(myPlayer->get_pos_x(), myPlayer->get_pos_y(), visionAngle,
                          jugador.get_pos_x(), jugador.get_pos_y())) {
            player.drawPlayer(jugador, anglePlayer);
        }
    }

    int time;
    int tiempoPartida = mensaje->get_round_time();
    int round = mensaje->get_round();

    if (is_bomb_planted) {
        time = mensaje->get_bomb_timer();
    } else {
        time = tiempoPartida;
    }

    SDL_Rect mouse = {mouseX, mouseY, 40, 40};
    hud.draw(mouse, time, round, *myPlayer, is_bomb_planted);

    if (mensaje->is_round_ended()) {
        Team team = mensaje->get_round_winner();
        mapa.drawEndRound(team, mensaje->is_bomb_planted());

        bloodStains.clear();
        lastHealths.clear();
    }
    if (tiempoPartida >= configuracion.tiempoDeCompra && myPlayer->get_status() == Status::Alive) {
        mapa.drawCampField(myAngle, myPlayer->get_pos_x(), myPlayer->get_pos_y());
    } else if (tiempoPartida < configuracion.tiempoDeCompra) {
        mapa.drawShop();
    }

    mapa.drawExplosion();

    std::list<DroppedWeapon> dropped_weapons = mensaje->get_dropped_weapons();
    for (auto weapon: dropped_weapons) {
        if (puntoEnVision(myPlayer->get_pos_x(), myPlayer->get_pos_y(), visionAngle,
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
        SDL_Rect screen = {0, 0, configuracion.widthWindow, configuracion.heightWindow};
        sdlRenderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        sdlRenderer->SetDrawColor(255, 0, 0, 120);  // Rojo semi-transparente
        sdlRenderer->FillRect(screen);
        damageFlashTimer--;
    }

    sdlRenderer->Present();
}

double Render::getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey) {

    // Calcular el ángulo entre el jugador y el mouse
    int dx = mousex - jugadorX;
    int dy = mousey - jugadorY;

    double angleRadians = atan2(dy, dx);  // da un ángulo en radianes entre -π y π
    double angleDegrees = angleRadians * 180 / M_PI;

    return angleDegrees;  // Ajustar para que apunte hacia la dirección del mouse
}

bool Render::puntoEnVision(int playerX, int playerY, float visionAngleDeg, int puntoX, int puntoY) {
    float fovDeg = configuracion.conoVision;
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
    visionAngleDeg = normalizarAngulo(visionAngleDeg);
    angleToPoint = normalizarAngulo(angleToPoint);

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

float Render::normalizarAngulo(float angulo) {
    while (angulo < 0) angulo += 360;
    while (angulo >= 360) angulo -= 360;
    return angulo;
}

void Render::clearScreen() {
    sdlRenderer->SetDrawColor(0, 0, 0, 255);
    sdlRenderer->Clear();
}
