#ifndef HUDSPRITE_H
#define HUDSPRITE_H

#include "component.h"

class HudSprite : public Component{

    private:
    int& weidthScreen;
    int& heightScreen;

    void drawSymbols(int& num, int& posSymbol, int& SymbolX, int& SymbolY);

    public:
    HudSprite(Renderer* sdlRenderer, int& weidth, int& height);

    void draw(SDL_Rect& mouse, int& health, int& money, int& time);
};
#endif
