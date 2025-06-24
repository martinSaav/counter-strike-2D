#ifndef HUDSPRITE_H
#define HUDSPRITE_H

#include "component.h"

enum HudType {
    HEALTH = 0,
    TIME = 2,
    SHOP = 4,
    BOMB = 6,
    MONEY = 7,
    DOBLEPUNTO = 10
};

// huds Sobre la pantalla
class HudSprite : public Component{

    private:
    int alturaSymbols = 64;
    int anchoSymbols = 64;
    int anchoSymbolPantalla = 40;
    int altoSymbolPantalla = 50;

    int anchoNum = 50;
    int alturaNum = 66;

    int alturaWeapons = 17;
    int anchoSlotPantalla = 200;

    // Balas
    int alturaBullets = 50;
    int anchoBullets = 50;
    int alturaBulletsPantalla = 40;
    int anchoBulletsPantalla = 40;

    int anchoBulletNum = 38;
    int anchoBulletNumPantalla = 20;
    int altoBulletNumPantalla = 20;

    // Shop
    int altoShopPantalla = 600;
    int anchoShopPantalla = 600;

    SDL_Rect srcRect;
    SDL_Rect destRect;

    std::string textureName;
    
    void drawHuds(int num, HudType tipo, int& symbolX, int& symbolY);

    void drawWeapon(int symbolX, int symbolY, int& anchoWeapon, int& weaponTextureX,
        int& ammoWeapon, Weapon& weaponPLayer);

    void castCordinates(Weapon& myWeapon, int& anchoWeapon, int& weaponTextureX);

    void castSizesWeapon(Weapon& myWeapon, int& anchoWeapon, int& altoWeapon, std::string& textureName);

    void drawSounds(PlayerInfo& myPlayer);

    public:
    HudSprite(Renderer* sdlRenderer, Configuracion& configuracion);

    void draw(SDL_Rect& mouse, int& time, int& round, PlayerInfo& myPlayer, bool& is_bomb_planted);

    void drawWeaponDroped(Weapon weapon, int weaponX, int weaponY);
};
#endif
