#include "mapSprite.h"

MapSprite::MapSprite(Renderer* sdlRenderer, Configuracion& configuracion)
    : Component(sdlRenderer, configuracion){

    texturas.loadTexture("map", "../client/data/maps/default_aztec.png");

    // round
    texturas.loadTexture("terroristWins", "../client/data/hud/terroristWins.png");
    texturas.loadTexture("counterterroristWins", "../client/data/hud/counterTerroristWins.png");

    // bomb
    texturas.loadTexture("bomb", "../client/data/weapons/bomb.bmp");

    // camp field
    texturas.loadTexture("field", "../client/data/maps/field.png");

    tipoMusic music = AMBIENTE;
    int cantVeces = 4;
    sounds.loadMusic(music, cantVeces);
}

void MapSprite::draw(){
    std::string textureName = "map";
    int camWidth = configuracion.widthWindow / configuracion.zoom;
    int camHeight = configuracion.heightWindow / configuracion.zoom;

    SDL_Rect srcRect = {configuracion.camera.x, configuracion.camera.y, camWidth, camHeight};

    SDL_Rect destRect = {
        0, 0,
        configuracion.widthWindow,
        configuracion.heightWindow
    };
    drawHud( srcRect, destRect, textureName);
}

int MapSprite::getWidth(){
    Texture& map = texturas.getTexture("map");
    int worldWidth = map.GetWidth();

    return worldWidth;
}

int MapSprite::getHeight(){
    Texture& map = texturas.getTexture("map");
    int worldHeight = map.GetHeight();

    return worldHeight;
}

void MapSprite::drawEndRound(Team& team, int zoom){
    std::string textureName;
    int anchoCartelPantalla = anchoCartel * zoom / 8;
    int symbolX = (configuracion.widthWindow * 0.5) - anchoCartelPantalla / 2;
    int symbolY = configuracion.heightWindow * 0.2;

    SDL_Rect destRect = {
        symbolX,
        symbolY,
        anchoCartelPantalla,
        int(altoCartel * zoom / 8)
    };

    switch (team){
    case Team::Terrorists:
        textureName = "terroristWins";
        break;
    case Team::CounterTerrorists:
        textureName = "counterterroristWins";
        break;
    }
    drawHud2(destRect, textureName);
}

bool MapSprite::isBombActivated(){
   return this->is_bomb_activated;
}

void MapSprite::activateBomb(){
    this->is_bomb_activated = true;

    tipoMusic music = TIMER;
    // Dura 2 segundos
    int cantVeces = 8;
    sounds.loadSong(music);
}

void MapSprite::desactivateBomb(){
    this->is_bomb_activated = false;
    sounds.stopAllSongs();
}

void MapSprite::drawBomb(int bomb_x, int bomb_y){

    SDL_Rect destRectBomb = {
        int((bomb_x - configuracion.camera.x) * configuracion.zoom),
        int((bomb_y - configuracion.camera.y) * configuracion.zoom),
        int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)
    };
    std::string textureName = "bomb";
    drawHud2(destRectBomb, textureName);
}

void MapSprite::drawCampField(int angle, int playerX, int playerY){
    Texture& field = texturas.getTexture("field");

    // coregimos el anglo por la imagen
    angle -= 90;

    // Dimensiones reales de la textura de campo de visión
    int playerCenterX = playerX + int(30 / 8) / 2;
    int playerCenterY = playerY + int(30 / 8) / 2;

    // Posición del jugador en la pantalla (en píxeles)
    int playerScreenX = int((playerCenterX - configuracion.camera.x) * configuracion.zoom);
    int playerScreenY = int((playerCenterY - configuracion.camera.y) * configuracion.zoom);

    SDL_Rect destRect = {
        playerScreenX - anchoField / 2,
        playerScreenY - altoField / 2,
        anchoField,
        altoField
    };
    // Centro de la textura
    SDL_Point center = {anchoField / 2, altoField / 2};
    sdlRenderer->Copy(field, SDL2pp::NullOpt, destRect, angle, center);
}
