#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include <string>

#include "component.h"

class PlayerSprite: public Component {

private:
    SDL_Rect srcRect = {0, 0, 32, 32};

    void drawWeapon(int& playerX, int& playerY, double& angle, Weapon& weaponPlayer);

    void drawBullet(const int& shootX, const int& shootY, double& angle);

    std::string castWeapon(const Weapon& weapon);

    MusicType castShoot(const Weapon& weapon);

public:
    PlayerSprite(Renderer* sdlRenderer, Configuration& configuration, std::string& assetsPath);

    void drawPlayer(const PlayerInfo& player, double& angle);

    void drawPlayerDeath(int playerX, int playerY);

    void drawShotgunSpread(int playerX, int playerY, int shootX, int shootY);

    void drawBlood(int x, int y);
};
#endif
