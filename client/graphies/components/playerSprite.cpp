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
    texturas.loadTexture("tiro", "../client/data/hud/explocion.png");

    // Death
    texturas.loadTexture("hud_symbols", "../client/data/hud/hud_symbols.png");
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

    if (jugador.get_action() == Action::Shoot){
        int shootX = jugador.get_pos_shoot_x();
        int shootY = jugador.get_pos_shoot_y();
        drawBullet(camera, zoom, shootX, shootY, angle);
    }
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

    SDL_Rect destRect = {
        int((jugadorX + rotateDesvioX - camera.x) * zoom),
        int((jugadorY + rotateDesvioY - camera.y) * zoom), int(32 * zoom / 4),
        int(32 * zoom / 4)};

    sdlRenderer->Copy(weapon, srcRect, destRect, angle);
}

void PlayerSprite::drawBullet(SDL_Rect& camera, float& zoom, int& shootX, int& shootY, double& angle) {

    SDL_Rect srcRectBullet = {0,0,360,360};

    SDL_Rect destRect = {
    int((shootX - camera.x) * zoom),
    int((shootY - camera.y) * zoom),
    int(4 * zoom),
    int(4 * zoom)
    };
    
    Texture& tiro = texturas.getTexture("tiro");
    sdlRenderer->Copy(tiro, srcRectBullet, destRect);
}

void PlayerSprite::drawPlayerDeath(int jugadorX, int jugadorY, SDL_Rect& camera, float& zoom) {
    Texture& hud_symbols = texturas.getTexture("hud_symbols");
    hud_symbols.SetColorMod(255, 0, 0);

    int posSymbolDeath = 12; 
    SDL_Rect srcRectDeath = {64 * 12,0,64,64};

    SDL_Rect destRect = {
    int((jugadorX - camera.x) * zoom),
    int((jugadorY - camera.y) * zoom),
    int(32 * zoom / 6), int(32 * zoom / 6)};
    
    sdlRenderer->Copy(hud_symbols, srcRectDeath, destRect);
}
