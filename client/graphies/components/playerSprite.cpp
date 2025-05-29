#include "playerSprite.h"

   
PlayerSprite::PlayerSprite(Renderer* sdlRenderer)
    : Component(sdlRenderer){

    texturas.loadTexture("t1", "../client/data/players/t1.bmp");
    texturas.loadTexture("t2", "../client/data/players/t2.bmp");
    texturas.loadTexture("t3", "../client/data/players/t3.bmp");
    texturas.loadTexture("t4", "../client/data/players/t4.bmp");

    texturas.loadTexture("c1", "../client/data/players/ct1.bmp");
    texturas.loadTexture("c2", "../client/data/players/ct2.bmp");
    texturas.loadTexture("c3", "../client/data/players/ct3.bmp");
    texturas.loadTexture("c4", "../client/data/players/ct4.bmp");
}

void PlayerSprite::draw(int& posX ,int& posY ,SDL_Rect& camera ,float& zoom ,double& angle){

    Texture& personaje = texturas.getTexture("t1");

    SDL_Rect destRect = {
        int((posX - camera.x) * zoom),
        int((posY - camera.y) * zoom),
        int(32 * zoom / 4),
        int(32 * zoom / 4)
    };

    sdlRenderer->Copy(personaje, srcRect, destRect, angle);

}