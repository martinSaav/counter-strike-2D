#ifndef MAPSPRITE_H
#define MAPSPRITE_H

#include "component.h"

class MapSprite : public Component{

    private:
    SDL_Rect destRect = {0, 0, 1000, 1000};

    public:
    MapSprite(Renderer* sdlRenderer);

    void draw(SDL_Rect& camera ,int& camWidth ,int& camHeight);

    int getWidth();

    int getHeight();
};
#endif
