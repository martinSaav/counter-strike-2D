#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <SDL.h>
#include "../../common/dto/game_config_info.h"

struct Configuracion{

    GameConfigInfo* gameConfig;
    SDL_Rect camera = {0, 0, 0, 0};
    float zoom = 20.0f;
    int widthWindow, heightWindow;
    float tiempoDeCompra;
    int opacidadVision;
    float conoVision = 70.0f;

    // Constructor
    Configuracion(int width, int height, GameConfigInfo* gameConfig) : gameConfig(gameConfig){
        widthWindow = width;
        heightWindow = height;

        int camWidth = widthWindow / zoom;
        int camHeight = heightWindow / zoom;
        camera = {0, 0, camWidth, camHeight};

        tiempoDeCompra = gameConfig->get_buy_time() / 1000.0f;
        opacidadVision = gameConfig->get_opacity();
        //conoVision = gameConfig->get_cone_angle();
    }
};
#endif
