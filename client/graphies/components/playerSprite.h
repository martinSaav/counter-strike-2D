#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include <string>

#include "component.h"

class PlayerSprite: public Component {

private:
    SDL_Rect srcRect = {0, 0, 32, 32};

    void drawWeapon(int& jugadorX, int& jugadorY, double& angle, Weapon& weaponPlayer);

    void drawBullet(const int& shootX, const int& shootY, double& angle);

    std::string castWeapon(const Weapon& weapon);

    tipoMusic castShoot(const Weapon& weapon);

public:
    PlayerSprite(Renderer* sdlRenderer, Configuracion& configuracion);

    void drawPlayer(const PlayerInfo& jugador, double& angle);

    void drawPlayerDeath(int jugadorX, int jugadorY);

    void drawBlood(int x, int y);
};
#endif
