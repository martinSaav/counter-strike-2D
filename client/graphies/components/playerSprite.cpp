#include "playerSprite.h"

#include <cmath>

PlayerSprite::PlayerSprite(Renderer* sdlRenderer): Component(sdlRenderer) {

    // Skins
    texturas.loadTexture("T1", "../client/data/players/t1.bmp");
    texturas.loadTexture("T2", "../client/data/players/t2.bmp");
    texturas.loadTexture("T3", "../client/data/players/t3.bmp");
    texturas.loadTexture("T4", "../client/data/players/t4.bmp");

    texturas.loadTexture("C1", "../client/data/players/ct1.bmp");
    texturas.loadTexture("C2", "../client/data/players/ct2.bmp");
    texturas.loadTexture("C3", "../client/data/players/ct3.bmp");
    texturas.loadTexture("C4", "../client/data/players/ct4.bmp");

    // Weapons
    texturas.loadTexture("ak47", "../client/data/weapons/ak47.bmp");
    texturas.loadTexture("awp", "../client/data/weapons/awp.bmp");
    texturas.loadTexture("m3", "../client/data/weapons/m3.bmp");
    texturas.loadTexture("glock", "../client/data/weapons/glock.bmp");
    texturas.loadTexture("knife", "../client/data/weapons/knife.bmp");

    // Bullets
    texturas.loadTexture("shade", "../client/data/hud/hud_shade.bmp");
}

void PlayerSprite::drawPlayer(const PlayerInfo& jugador, SDL_Rect& camera ,float& zoom, double& angle) {

    int jugadorX = jugador.get_pos_x();
    int jugadorY = jugador.get_pos_y();
    std::string skin = jugador.get_skin();
    Texture& personaje = texturas.getTexture(skin);

    SDL_Rect destRect = {
        int((jugadorX - camera.x) * zoom),
        int((jugadorY - camera.y) * zoom),
        int(32 * zoom / 4), int(32 * zoom / 4)};

    sdlRenderer->Copy(personaje, srcRect, destRect, angle);

    std::string weaponName = "ak47";
    this->drawWeapon(jugadorX, jugadorY, camera, zoom, angle, weaponName);

    bool hayDisparo = true;
    //if (hayDisparo){
    //    int shootX = 40;
    //    int shootY = 40;
    //    drawBullet(jugadorX, jugadorY, camera, zoom, shootX, shootY, angle);
    //}
}

void PlayerSprite::drawWeapon(int& jugadorX, int& jugadorY, SDL_Rect& camera, float& zoom,
        double& angle, std::string& weaponName) {

    Texture& weapon = texturas.getTexture(weaponName);

    // Offset desde el centro del personaje hacia la mano, sin rotar
    float desvioX = 0.0f;   // derecha
    float desvioY = -6.0f;  // hacia arriba

    // Convertir el ángulo a radianes
    float radians = angle * M_PI / 180.0f;

    // Rotar el offset
    float rotateDesvioX = desvioX * cos(radians) - desvioY * sin(radians);
    float rotateDesvioY = desvioX * sin(radians) + desvioY * cos(radians);

    SDL_Rect destRect = {int((jugadorX + rotateDesvioX - camera.x) * zoom),
                         int((jugadorY + rotateDesvioY - camera.y) * zoom), int(32 * zoom / 4),
                         int(32 * zoom / 4)};

    sdlRenderer->Copy(weapon, srcRect, destRect, angle);
}

void PlayerSprite::drawBullet(int& jugadorX, int& jugadorY, SDL_Rect& camera, float& zoom,
        int& shootX, int& shootY, double& angle) {

    Texture& shade = texturas.getTexture("shade");

    angle += 90;
    int altoBala = 1;
    //int anchoBala = calcularDistancia(jugadorX, jugadorY, shootX, shootY);

    // Transformar a coordenadas de renderizado
    int renderJugadorX = int((jugadorX - camera.x) * zoom);
    int renderJugadorY = int((jugadorY - camera.y) * zoom);
    int renderShootX = int((shootX - camera.x) * zoom);
    int renderShootY = int((shootY - camera.y) * zoom);

    // Calcular distancia en pantalla
    int anchoBala = calcularDistancia(renderJugadorX, renderJugadorY, renderShootX, renderShootY);



    SDL_Rect srcRect = {0, 0, 250, altoBala};
    //SDL_Rect destRect = {jugadorX, jugadorY, anchoBala, altoBala};
    SDL_Rect destRect = {renderJugadorX, renderJugadorY, anchoBala, altoBala};

    sdlRenderer->Copy(shade, srcRect, destRect, angle);
}

float PlayerSprite::calcularDistancia(int& x1, int& y1, int& x2, int& y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}