#include "playerSprite.h"

#include <cmath>

PlayerSprite::PlayerSprite(Renderer* sdlRenderer, Configuration& configuration,
                           std::string& assetsPath):
        Component(sdlRenderer, configuration, assetsPath) {

    // Skins
    textures.loadTexture("T1", assetsPath + "/players/t1.bmp");
    textures.loadTexture("T2", assetsPath + "/players/t2.bmp");
    textures.loadTexture("T3", assetsPath + "/players/t3.bmp");
    textures.loadTexture("T4", assetsPath + "/players/t4.bmp");

    textures.loadTexture("C1", assetsPath + "/players/ct1.bmp");
    textures.loadTexture("C2", assetsPath + "/players/ct2.bmp");
    textures.loadTexture("C3", assetsPath + "/players/ct3.bmp");
    textures.loadTexture("C4", assetsPath + "/players/ct4.bmp");

    // Weapons
    textures.loadTexture("ak47", assetsPath + "/weapons/ak47.bmp");
    textures.loadTexture("awp", assetsPath + "/weapons/awp.bmp");
    textures.loadTexture("m3", assetsPath + "/weapons/m3.bmp");
    textures.loadTexture("glock", assetsPath + "/weapons/glock.bmp");
    textures.loadTexture("knife", assetsPath + "/weapons/knife.bmp");
    textures.loadTexture("bomb", assetsPath + "/weapons/bomb.bmp");

    // Bullets
    textures.loadTexture("shot", assetsPath + "/hud/explocion.png");

    // Death
    textures.loadTexture("hud_symbols", assetsPath + "/hud/hud_symbols.png");

    // Blood
    textures.loadTexture("blood", assetsPath + "/maps/blood.png");
}

void PlayerSprite::drawPlayer(const PlayerInfo& player, double& angle) {

    angle += 90;
    int playerX = player.get_pos_x();
    int playerY = player.get_pos_y();
    std::string skin = player.get_skin();
    Texture& character = textures.getTexture(skin);

    SDL_Rect destRect = {int((playerX - configuration.camera.x) * configuration.zoom),
                         int((playerY - configuration.camera.y) * configuration.zoom),
                         int(32 * configuration.zoom / 8), int(32 * configuration.zoom / 8)};

    sdlRenderer->Copy(character, srcRect, destRect, angle);

    Weapon weaponPlayer = player.get_active_weapon();

    if (weaponPlayer == Weapon::None) {
        return;  // No tiene arma
    }
    this->drawWeapon(playerX, playerY, angle, weaponPlayer);

    if (player.get_action() == Action::Shoot) {

        int volume = 64;

        if (weaponPlayer == Weapon::Bomb) {
            return;
        } else if (weaponPlayer == Weapon::Knife) {
            volume = 90;
        }
        int shootX = player.get_pos_shoot_x();
        int shootY = player.get_pos_shoot_y();

        drawBullet(shootX, shootY, angle);

        MusicType music = castShoot(weaponPlayer);
        sounds.loadSong(music, 0, volume);
    }
}

void PlayerSprite::drawWeapon(int& playerX, int& playerY, double& angle, Weapon& weaponPlayer) {

    std::string weaponName = castWeapon(weaponPlayer);
    Texture& weaponTexture = textures.getTexture(weaponName);
    // Offset desde el centro del personaje hacia la mano, sin rotar
    float offsetX = 0.0f;  // derecha
    float offsetY = -2.2f;
    if (weaponName == "ak47") {
        offsetY = -3.0f;  // hacia arriba
    }

    // angulo a radianes
    float radians = angle * M_PI / 180.0f;

    // Rotar el offset
    float rotatedOffsetX = offsetX * cos(radians) - offsetY * sin(radians);
    float rotatedOffsetY = offsetX * sin(radians) + offsetY * cos(radians);

    SDL_Rect destRect = {
            int((playerX + rotatedOffsetX - configuration.camera.x) * configuration.zoom),
            int((playerY + rotatedOffsetY - configuration.camera.y) * configuration.zoom),
            int(32 * configuration.zoom / 8), int(32 * configuration.zoom / 8)};

    sdlRenderer->Copy(weaponTexture, SDL2pp::NullOpt, destRect, angle);
}

void PlayerSprite::drawBullet(const int& shootX, const int& shootY, double& angle) {

    SDL_Rect destRect = {int((shootX - configuration.camera.x) * configuration.zoom),
                         int((shootY - configuration.camera.y) * configuration.zoom),
                         int(2 * configuration.zoom), int(2 * configuration.zoom)};
    std::string textureName = "shot";
    drawHud2(destRect, textureName);
}

void PlayerSprite::drawPlayerDeath(int playerX, int playerY) {
    Texture& hud_symbols = textures.getTexture("hud_symbols");
    std::string textureName = "hud_symbols";
    hud_symbols.SetColorMod(255, 0, 0);

    SDL_Rect srcRectDeath = {64 * 12, 0, 64, 64};

    SDL_Rect destRect = {int((playerX - configuration.camera.x) * configuration.zoom),
                         int((playerY - configuration.camera.y) * configuration.zoom),
                         int(32 * configuration.zoom / 8), int(32 * configuration.zoom / 8)};
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

MusicType PlayerSprite::castShoot(const Weapon& weapon) {
    switch (weapon) {
        case Weapon::Knife:
            return KNIFE;
        case Weapon::Glock:
            return SHOT_PISTOL;
        case Weapon::AK47:
            return SHOT_AK47;
        case Weapon::AWP:
            return SHOT_AWP;
        case Weapon::M3:
            return SHOT_M3;
    }
    return SHOT_PISTOL;
}

void PlayerSprite::drawBlood(int x, int y) {
    Texture& blood = textures.getTexture("blood");

    SDL_Rect destRect = {int((x - configuration.camera.x) * configuration.zoom),
                         int((y - configuration.camera.y) * configuration.zoom),
                         int(32 * configuration.zoom / 8),  // ajustá tamaño según la textura
                         int(32 * configuration.zoom / 8)};

    sdlRenderer->Copy(blood, SDL2pp::NullOpt, destRect);
}
