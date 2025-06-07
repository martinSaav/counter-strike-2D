#include "playerSprite.h"
#include <cmath>
   
PlayerSprite::PlayerSprite(Renderer* sdlRenderer)
    : Component(sdlRenderer){

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
}

void PlayerSprite::drawPlayer(int& jugadorX, int& jugadorY, SDL_Rect& camera,
     float& zoom, double& angle, std::string& skin){

    Texture& personaje = texturas.getTexture("C1");

    SDL_Rect destRect = {
        int((jugadorX - camera.x) * zoom),
        int((jugadorY - camera.y) * zoom),
        int(32 * zoom / 4),
        int(32 * zoom / 4)
    };

    sdlRenderer->Copy(personaje, srcRect, destRect, angle);

    std::string weaponName = "ak47";
    this->drawWeapon(jugadorX, jugadorY, camera, zoom, angle, weaponName);
}

void PlayerSprite::drawWeapon(int& jugadorX, int& jugadorY, SDL_Rect& camera,
     float& zoom, double& angle, std::string& weaponName){

    Texture& weapon = texturas.getTexture(weaponName);
    
    // Offset desde el centro del personaje hacia la mano, sin rotar
    float desvioX = 0.0f;  // derecha
    float desvioY = -6.0f;  // hacia arriba

    // Convertir el ángulo a radianes
    float radians = angle * M_PI / 180.0f;

    // Rotar el offset
    float rotateDesvioX = desvioX * cos(radians) - desvioY * sin(radians);
    float rotateDesvioY = desvioX * sin(radians) + desvioY * cos(radians);

    SDL_Rect destRect = {
        int((jugadorX + rotateDesvioX - camera.x) * zoom),
        int((jugadorY + rotateDesvioY - camera.y) * zoom),
        int(32 * zoom / 4),
        int(32 * zoom / 4)
    };

    sdlRenderer->Copy(weapon, srcRect, destRect, angle);
}
