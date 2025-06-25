#include "playerSprite.h"

#include <cmath>

PlayerSprite::PlayerSprite(Renderer* sdlRenderer, Configuracion& configuracion):
        Component(sdlRenderer, configuracion) {

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

    // Blood
    texturas.loadTexture("blood", "../client/data/maps/blood.png");
}

void PlayerSprite::drawPlayer(const PlayerInfo& jugador, double& angle) {

    angle += 90;
    int jugadorX = jugador.get_pos_x();
    int jugadorY = jugador.get_pos_y();
    std::string skin = jugador.get_skin();
    Texture& personaje = texturas.getTexture(skin);

    SDL_Rect destRect = {int((jugadorX - configuracion.camera.x) * configuracion.zoom),
                         int((jugadorY - configuracion.camera.y) * configuracion.zoom),
                         int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)};

    sdlRenderer->Copy(personaje, srcRect, destRect, angle);

    Weapon weaponPlayer = jugador.get_active_weapon();

    if (weaponPlayer == Weapon::None) {
        return;  // No tiene arma
    }
    this->drawWeapon(jugadorX, jugadorY, angle, weaponPlayer);

    if (jugador.get_action() == Action::Shoot) {

        int volume = 64;

        if (weaponPlayer == Weapon::Bomb) {
            return;
        } else if (weaponPlayer == Weapon::Knife) {
            volume = 90;
        }
        int shootX = jugador.get_pos_shoot_x();
        int shootY = jugador.get_pos_shoot_y();

        drawBullet(shootX, shootY, angle);

        tipoMusic music = castShoot(weaponPlayer);
        sounds.loadSong(music, 0, volume);
    }
}

void PlayerSprite::drawWeapon(int& jugadorX, int& jugadorY, double& angle, Weapon& weaponPlayer) {

    std::string weaponName = castWeapon(weaponPlayer);
    Texture& weaponTexture = texturas.getTexture(weaponName);
    // Offset desde el centro del personaje hacia la mano, sin rotar
    float desvioX = 0.0f;  // derecha
    float desvioY = -2.2f;
    if (weaponName == "ak47") {
        desvioY = -3.0f;  // hacia arriba
    }

    // angulo a radianes
    float radians = angle * M_PI / 180.0f;

    // Rotar el offset
    float rotateDesvioX = desvioX * cos(radians) - desvioY * sin(radians);
    float rotateDesvioY = desvioX * sin(radians) + desvioY * cos(radians);

    SDL_Rect destRect = {
            int((jugadorX + rotateDesvioX - configuracion.camera.x) * configuracion.zoom),
            int((jugadorY + rotateDesvioY - configuracion.camera.y) * configuracion.zoom),
            int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)};

    sdlRenderer->Copy(weaponTexture, SDL2pp::NullOpt, destRect, angle);
}

void PlayerSprite::drawBullet(const int& shootX, const int& shootY, double& angle) {

    SDL_Rect destRect = {int((shootX - configuracion.camera.x) * configuracion.zoom),
                         int((shootY - configuracion.camera.y) * configuracion.zoom),
                         int(2 * configuracion.zoom), int(2 * configuracion.zoom)};
    std::string textureName = "tiro";
    drawHud2(destRect, textureName);
}

void PlayerSprite::drawPlayerDeath(int jugadorX, int jugadorY) {
    Texture& hud_symbols = texturas.getTexture("hud_symbols");
    std::string textureName = "hud_symbols";
    hud_symbols.SetColorMod(255, 0, 0);

    SDL_Rect srcRectDeath = {64 * 12, 0, 64, 64};

    SDL_Rect destRect = {int((jugadorX - configuracion.camera.x) * configuracion.zoom),
                         int((jugadorY - configuracion.camera.y) * configuracion.zoom),
                         int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)};
    drawHud(srcRectDeath, destRect, textureName);
}


std::string PlayerSprite::castWeapon(const Weapon& weapon) {
    switch (weapon) {
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
            return "None";
    }
}

tipoMusic PlayerSprite::castShoot(const Weapon& weapon) {
    switch (weapon) {
        case Weapon::Knife:
            return KNIFE;
        case Weapon::Glock:
            return DISPARO_PISTOL;
        case Weapon::AK47:
            return DISPARO_AK47;
        case Weapon::AWP:
            return DISPARO_AWP;
        case Weapon::M3:
            return DISPARO_M3;
    }
    return DISPARO_PISTOL;
}

void PlayerSprite::drawBlood(int x, int y) {
    Texture& blood = texturas.getTexture("blood");

    SDL_Rect destRect = {int((x - configuracion.camera.x) * configuracion.zoom),
                         int((y - configuracion.camera.y) * configuracion.zoom),
                         int(32 * configuracion.zoom / 8),  // ajustá tamaño según la textura
                         int(32 * configuracion.zoom / 8)};

    sdlRenderer->Copy(blood, SDL2pp::NullOpt, destRect);
}
