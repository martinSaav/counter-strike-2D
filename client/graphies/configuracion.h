#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <SDL.h>
#include "../../common/dto/game_config_info.h"

struct Configuracion{

    GameConfigInfo* gameConfig;
    SDL_Rect camera = {0, 0, 0, 0};
    float zoom = 20.0f;
    int widthWindow, heightWindow;
    int tiempoDeCompra = 20;
    int opacidadVision = 150;

    // Constructor
    Configuracion(int width, int height, GameConfigInfo* gameConfig) : gameConfig(gameConfig){
        widthWindow = width;
        heightWindow = height;

        int camWidth = widthWindow / zoom;
        int camHeight = heightWindow / zoom;
        camera = {0, 0, camWidth, camHeight};
    }
};
#endif
