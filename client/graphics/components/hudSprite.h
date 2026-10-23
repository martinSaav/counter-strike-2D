#ifndef HUDSPRITE_H
#define HUDSPRITE_H

#include <string>

#include "component.h"

enum HudType { HEALTH = 0, TIME = 2, SHOP = 4, BOMB = 6, MONEY = 7, COLON = 10 };

// huds Sobre la pantalla
class HudSprite: public Component {

private:
    int symbolsHeight = 64;
    int symbolsWidth = 64;
    int symbolScreenWidth = 40;
    int symbolScreenHeight = 50;

    int numWidth = 50;
    int numHeight = 66;

    int weaponsHeight = 17;
    int slotScreenWidth = 200;

    // Balas
    int bulletsHeight = 50;
    int bulletsWidth = 50;
    int bulletsScreenHeight = 40;
    int bulletsScreenWidth = 40;

    int bulletNumWidth = 38;
    int bulletNumScreenWidth = 20;
    int bulletNumScreenHeight = 20;

    // Shop
    int shopScreenHeight = 600;
    int shopScreenWidth = 600;

    SDL_Rect srcRect;
    SDL_Rect destRect;

    std::string textureName;

    void drawHuds(int num, HudType type, int& symbolX, int& symbolY);

    void drawWeapon(int symbolX, int symbolY, int& weaponWidth, int& weaponTextureX,
                    int& ammoWeapon, Weapon& weaponPlayer);

    void castCordinates(const Weapon& myWeapon, int& weaponWidth, int& weaponTextureX);

    void castSizesWeapon(const Weapon& myWeapon, int& weaponWidth, int& weaponHeight,
                         std::string& textureName);

    void drawSounds(PlayerInfo& myPlayer);

public:
    HudSprite(Renderer* sdlRenderer, Configuration& configuration, std::string& assetsPath);

    void draw(SDL_Rect& mouse, int& time, int& round, PlayerInfo& myPlayer, bool& is_bomb_planted,
              bool isBuyStage);

    void drawWeaponDroped(Weapon weapon, int weaponX, int weaponY);
};
#endif
