#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include "component.h"

class PlayerSprite : public Component{

    private:
    SDL_Rect srcRect = {0, 0, 32, 32};

    public:
    PlayerSprite(Renderer* sdlRenderer);

    void draw(int& posX ,int& posY ,SDL_Rect& camera ,float& zoom, double& angle, std::string& skin);
};
#endif
