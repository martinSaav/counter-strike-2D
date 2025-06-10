#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include "component.h"
#include "../../common/dto/player_info.h"

class PlayerSprite : public Component{

    private:
    SDL_Rect srcRect = {0, 0, 32, 32};

    void drawWeapon(int& jugadorX, int& jugadorY,double& angle, std::string& weaponName);

    void drawBullet(int& shootX, int& shootY, double& angle);

    public:
    PlayerSprite(Renderer* sdlRenderer, Configuracion& configuracion);

    void drawPlayer(const PlayerInfo& jugador, double& angle);

    void drawPlayerDeath(int jugadorX, int jugadorY);
};
#endif
