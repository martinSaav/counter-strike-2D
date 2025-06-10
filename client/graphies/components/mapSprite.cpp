#include "mapSprite.h"

MapSprite::MapSprite(Renderer* sdlRenderer, Configuracion& configuracion)
    : Component(sdlRenderer, configuracion){

    texturas.loadTexture("fondo", "../client/data/maps/default_aztec.png");

    // round
    texturas.loadTexture("terroristWins", "../client/data/hud/terroristWins.png");
    texturas.loadTexture("counterterroristWins", "../client/data/hud/counterTerroristWins.png");

    // bomb
    texturas.loadTexture("bomb", "../client/data/weapons/bomb.bmp");

    // shop
    texturas.loadTexture("shop", "../client/data/maps/shop2.png");

    // camp field
    texturas.loadTexture("field", "../client/data/maps/field2.png");
}

void MapSprite::draw(SDL_Rect& camera ,int& camWidth ,int& camHeight){

    Texture& fondo = texturas.getTexture("fondo");

    SDL_Rect srcRect = {camera.x, camera.y, camWidth, camHeight};
    sdlRenderer->Copy(fondo, srcRect, destRectMap);
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

void MapSprite::drawEndRound(Team& team, int weidthScreen, int heightScreen, int zoom){
    Texture& terroristWins = texturas.getTexture("terroristWins");
    Texture& counterterroristWins = texturas.getTexture("counterterroristWins");

    int anchoCartel = 210;
    int altoCartel = 50;
    int anchoCartelPantalla = anchoCartel * zoom / 8;
    int symbolX = (weidthScreen * 0.5) - anchoCartelPantalla / 2;
    int symbolY = heightScreen * 0.2;

    SDL_Rect srcRect = {0, 0, anchoCartel, 50};

    SDL_Rect destRect = {
        symbolX,
        symbolY,
        anchoCartelPantalla,
        int(altoCartel * zoom / 8)
    };

    switch (team){
    case Team::Terrorists:
        sdlRenderer->Copy(terroristWins, srcRect, destRect);
        break;
    case Team::CounterTerrorists:
        sdlRenderer->Copy(counterterroristWins, srcRect, destRect);
        break;
    }
}

bool MapSprite::isBombActivated(){
   return this->is_bomb_activated;
}

void MapSprite::activateBomb(){
    this->is_bomb_activated = true;

    tipoMusic music = TIMER;
    // Dura 2 segundos
    int cantVeces = 8;
    sounds.loadMusic(music, cantVeces);
}

void MapSprite::desactivateBomb(){
    this->is_bomb_activated = false;
    sounds.stopMusic();
}

void MapSprite::drawBomb(int bomb_x, int bomb_y){

    Texture& bomb = texturas.getTexture("bomb");

    SDL_Rect destRectBomb = {
    int((bomb_x - configuracion.camera.x) * configuracion.zoom),
    int((bomb_y - configuracion.camera.y) * configuracion.zoom),
    int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)
    };

    sdlRenderer->Copy(bomb, srcRectBomb, destRectBomb);
}

void MapSprite::drawShop(int weidthScreen, int heightScreen){
    Texture& shop = texturas.getTexture("shop");
    
    int altoCartel = 200;
    int anchoCartel = 500;

    int symbolX = (weidthScreen * 0.5) - anchoCartel * 2 / 2;
    int symbolY = heightScreen * 0.2;

    SDL_Rect srcRect = {0, 0, anchoCartel, altoCartel};
    SDL_Rect destRect = {
        symbolX,
        symbolY,
        anchoCartel * 2,
        altoCartel * 2
    };
    sdlRenderer->Copy(shop, srcRect, destRect);
}

void MapSprite::drawCampField(int angle, int playerX, int playerY){
    Texture& field = texturas.getTexture("field");

    // coregimos el anglo por la imagen
    angle -= 90;
    
    int altoCartel = 100;
    int anchoCartel = 100;

    // Dimensiones reales de la textura de campo de visión
    int anchoField = 2300;
    int altoField = 2300;

    int playerCenterX = playerX + int(30 / 8) / 2;
    int playerCenterY = playerY + int(30 / 8) / 2;


    // Posición del jugador en la pantalla (en píxeles)
    int playerScreenX = int((playerCenterX - configuracion.camera.x) * configuracion.zoom);
    int playerScreenY = int((playerCenterY - configuracion.camera.y) * configuracion.zoom);

    // Mover la textura para que su centro esté donde está el jugador
    SDL_Rect srcRect = {0, 0, anchoCartel, altoCartel}; // Si estás usando toda la imagen, podrías usar NULL
    SDL_Rect destRect = {
        playerScreenX - anchoField / 2,
        playerScreenY - altoField / 2,
        anchoField,
        altoField
    };

    SDL_Point center = {anchoField / 2, altoField / 2}; // Centro de la textura

    sdlRenderer->Copy(field, srcRect, destRect, angle, center);
}
