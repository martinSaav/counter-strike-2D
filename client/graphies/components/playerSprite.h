#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include "component.h"
#include "../../common/dto/player_info.h"

class PlayerSprite : public Component{

    private:
    SDL_Rect srcRect = {0, 0, 32, 32};

    float calcularDistancia(int& x1, int& y1, int& x2, int& y2);

    public:
    PlayerSprite(Renderer* sdlRenderer);

    void drawPlayer(const PlayerInfo& jugador, SDL_Rect& camera ,float& zoom, double& angle);

    void drawWeapon(int& jugadorX, int& jugadorY, SDL_Rect& camera, float& zoom,double& angle, std::string& weaponName);

    void drawBullet(int& jugadorX, int& jugadorY, SDL_Rect& camera, float& zoom,
        int& shootX, int& shootY, double& angle);
};
#endif
