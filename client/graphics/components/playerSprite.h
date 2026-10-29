#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include <string>
#include <unordered_map>
#include <utility>

#include "component.h"

class PlayerSprite: public Component {

private:
    // Ultimo disparo de M3 de cada jugador y hasta cuando se dibuja
    std::unordered_map<std::string, std::pair<SDL_Point, Uint32>> recentSpreads;
    SDL_Rect srcRect = {0, 0, 32, 32};

    void drawWeapon(int& playerX, int& playerY, double& angle, Weapon& weaponPlayer);

    void drawBullet(const int& shootX, const int& shootY, double& angle);

    std::string castWeapon(const Weapon& weapon);

    MusicType castShoot(const Weapon& weapon);

public:
    PlayerSprite(Renderer* sdlRenderer, Configuration& configuration, TextureStore& textures,
                 MusicManager& sounds, const std::string& assetsPath);

    void drawPlayer(const PlayerInfo& player, double angle);

    void drawPlayerDeath(int playerX, int playerY);

    void drawShotgunSpread(int playerX, int playerY, int shootX, int shootY);

    void playShotSound(const PlayerInfo& player, float distance);

    void playSound(MusicType music, int baseVolume, float distance);

    void drawBlood(int x, int y);
};
#endif
