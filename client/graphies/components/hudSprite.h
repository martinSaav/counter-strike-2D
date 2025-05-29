#ifndef HUDSPRITE_H
#define HUDSPRITE_H

#include "component.h"

class HudSprite : public Component{

    private:
    int& weidthScreen;
    int& heightScreen;

    public:
    HudSprite(Renderer* sdlRenderer, int& weidth, int& height);

    void draw(SDL_Rect& mouse, int health, int money);
};
#endif
