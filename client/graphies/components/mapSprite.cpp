#include "mapSprite.h"

MapSprite::MapSprite(Renderer* sdlRenderer)
    : Component(sdlRenderer){

    texturas.loadTexture("fondo", "../client/data/maps/default_aztec.png");
}

void MapSprite::draw(SDL_Rect& camera ,int& camWidth ,int& camHeight){

    Texture& fondo = texturas.getTexture("fondo");

    SDL_Rect srcRect = {camera.x, camera.y, camWidth, camHeight};
    sdlRenderer->Copy(fondo, srcRect, destRect);
}

int MapSprite::getWidth(){
    Texture& fondo = texturas.getTexture("fondo");

    int worldWidth = fondo.GetWidth();
    return worldWidth;
}

int MapSprite::getHeight(){
    Texture& fondo = texturas.getTexture("fondo");
    
    int worldHeight = fondo.GetHeight();
    return worldHeight;
}