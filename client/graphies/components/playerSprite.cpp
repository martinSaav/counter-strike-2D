#include "playerSprite.h"

#include <cmath>

PlayerSprite::PlayerSprite(Renderer* sdlRenderer, Configuracion& configuracion): 
    Component(sdlRenderer, configuracion){

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
    texturas.loadTexture("bomb", "../client/data/weapons/bomb.bmp");

    // Bullets
    texturas.loadTexture("tiro", "../client/data/hud/explocion.png");

    // Death
    texturas.loadTexture("hud_symbols", "../client/data/hud/hud_symbols.png");
}

void PlayerSprite::drawPlayer(const PlayerInfo& jugador, double& angle) {

    angle += 90;
    int jugadorX = jugador.get_pos_x();
    int jugadorY = jugador.get_pos_y();
    std::string skin = jugador.get_skin();
    Texture& personaje = texturas.getTexture(skin);

    SDL_Rect destRect = {
        int((jugadorX - configuracion.camera.x) * configuracion.zoom),
        int((jugadorY - configuracion.camera.y) * configuracion.zoom),
        int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)};

    sdlRenderer->Copy(personaje, srcRect, destRect, angle);

    Weapon weaponPlayer = jugador.get_active_weapon();
    this->drawWeapon(jugadorX, jugadorY, angle, weaponPlayer);

    if (jugador.get_action() == Action::Shoot){
        int shootX = jugador.get_pos_shoot_x();
        int shootY = jugador.get_pos_shoot_y();
        drawBullet(shootX, shootY, angle);

        tipoMusic music = castShoot(weaponPlayer);
        int cantVeces = 1;
        sounds.loadMusic(music, cantVeces);
    }
}

void PlayerSprite::drawWeapon(int& jugadorX, int& jugadorY, double& angle, Weapon& weaponPlayer) {

    std::string weaponName = castWeapon(weaponPlayer);
    Texture& weaponTexture = texturas.getTexture(weaponName);
    // Offset desde el centro del personaje hacia la mano, sin rotar
    float desvioX = 0.0f;   // derecha
    float desvioY = -2.2f;
    if (weaponName == "ak47"){
        desvioY = -3.0f;  // hacia arriba
    }
    

    // Convertir el ángulo a radianes
    float radians = angle * M_PI / 180.0f;

    // Rotar el offset
    float rotateDesvioX = desvioX * cos(radians) - desvioY * sin(radians);
    float rotateDesvioY = desvioX * sin(radians) + desvioY * cos(radians);

    SDL_Rect destRect = {
        int((jugadorX + rotateDesvioX - configuracion.camera.x) * configuracion.zoom),
        int((jugadorY + rotateDesvioY - configuracion.camera.y) * configuracion.zoom),
        int(32 * configuracion.zoom / 8),
        int(32 * configuracion.zoom / 8)
    };

    sdlRenderer->Copy(weaponTexture, SDL2pp::NullOpt, destRect, angle);
}

void PlayerSprite::drawBullet(int& shootX, int& shootY, double& angle) {

    SDL_Rect destRect = {
    int((shootX - configuracion.camera.x) * configuracion.zoom),
    int((shootY - configuracion.camera.y) * configuracion.zoom),
    int(2 * configuracion.zoom),
    int(2 * configuracion.zoom)
    };
    
    Texture& tiro = texturas.getTexture("tiro");
    sdlRenderer->Copy(tiro, SDL2pp::NullOpt, destRect);
}

void PlayerSprite::drawPlayerDeath(int jugadorX, int jugadorY) {
    Texture& hud_symbols = texturas.getTexture("hud_symbols");
    hud_symbols.SetColorMod(255, 0, 0);

    int posSymbolDeath = 12; 
    SDL_Rect srcRectDeath = {64 * 12,0,64,64};

    SDL_Rect destRect = {
    int((jugadorX - configuracion.camera.x) * configuracion.zoom),
    int((jugadorY - configuracion.camera.y) * configuracion.zoom),
    int(32 * configuracion.zoom / 8),
    int(32 * configuracion.zoom / 8)
    };
    
    sdlRenderer->Copy(hud_symbols, srcRectDeath, destRect);
}


std::string PlayerSprite::castWeapon(Weapon& weapon){
    switch (weapon)
    {
    case Weapon::Knife:
        return "knife";
    case Weapon::Bomb:
        return "bomb";
    case Weapon::Glock:
        return "glock";
    case Weapon::AK47:
        return "ak47";
    case Weapon::M3:
        return "m3";
    case Weapon::AWP:
        return "awp";
    default:
        // None
        return "None";
    }
}

tipoMusic PlayerSprite::castShoot(Weapon& weapon){
    switch (weapon)
    {
    case Weapon::Knife:
        return KNIFE;
    case Weapon::Glock:
        return DISPARO_PISTOL;
    case Weapon::AK47:
        return DISPARO_AK47;
    }
    return NONE;
}
