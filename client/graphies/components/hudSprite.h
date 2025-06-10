#ifndef HUDSPRITE_H
#define HUDSPRITE_H

#include "component.h"

enum HudType {
    HEALTH = 0,
    TIME = 2,
    SHOP = 4,
    MONEY = 7,
    DOBLEPUNTO = 10
};

class HudSprite : public Component{

    private:
    int& weidthScreen;
    int& heightScreen;
    int alturaSymbols = 64;
    int anchoSymbols = 64;
    int anchoNum = 50;
    int alturaNum = 66;
    int anchoSymbolPantalla = 40;
    int altoSymbolPantalla = 50;
    
    void drawHuds(int& num, HudType tipo, int& symbolX, int& symbolY);

    void drawSymbol(int& posSymbol, int& symbolX, int& symbolY, Texture& hud_symbols);

    void drawNum(int& num, int& symbolX, int& symbolY, Texture& nums);

    void drawNumRound(int& round, int& symbolX, int& symbolY);

    void drawWeapon(int& symbolX, int& symbolY, int& anchoWeapons,
        int& weaponX, int& cantBalas);

    public:
    HudSprite(Renderer* sdlRenderer, Configuracion& configuracion, int& weidth, int& height);

    void draw(SDL_Rect& mouse, int& health, int& money, int& time, int& round);

};
#endif
