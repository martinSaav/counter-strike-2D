#include "playerSprite.h"

   
PlayerSprite::PlayerSprite(Renderer* sdlRenderer)
    : Component(sdlRenderer){

    texturas.loadTexture("T1", "../client/data/players/t1.bmp");
    texturas.loadTexture("T2", "../client/data/players/t2.bmp");
    texturas.loadTexture("T3", "../client/data/players/t3.bmp");
    texturas.loadTexture("T4", "../client/data/players/t4.bmp");

    texturas.loadTexture("C1", "../client/data/players/ct1.bmp");
    texturas.loadTexture("C2", "../client/data/players/ct2.bmp");
    texturas.loadTexture("C3", "../client/data/players/ct3.bmp");
    texturas.loadTexture("C4", "../client/data/players/ct4.bmp");
}

void PlayerSprite::draw(int& posX, int& posY, SDL_Rect& camera,
     float& zoom,double& angle, std::string& skin){

    Texture& personaje = texturas.getTexture(skin);

    SDL_Rect destRect = {
        int((posX - camera.x) * zoom),
        int((posY - camera.y) * zoom),
        int(32 * zoom / 4),
        int(32 * zoom / 4)
    };

    sdlRenderer->Copy(personaje, srcRect, destRect, angle);

}