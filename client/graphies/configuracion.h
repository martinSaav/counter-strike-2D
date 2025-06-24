#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <SDL.h>

#include "../../common/dto/game_config_info.h"

struct Configuracion {

    GameConfigInfo* gameConfig;
    SDL_Rect camera = {0, 0, 0, 0};
    float zoom = 15.0f;
    int widthWindow, heightWindow;
    float tiempoDeCompra;
    int opacidadVision;
    int conoVision;

    // Constructor
    Configuracion(int width, int height, GameConfigInfo* gameConfig): gameConfig(gameConfig) {
        widthWindow = width;
        heightWindow = height;

        int camWidth = widthWindow / zoom;
        int camHeight = heightWindow / zoom;
        camera = {0, 0, camWidth, camHeight};

        tiempoDeCompra = gameConfig->get_buy_time() / 1000.0f;
        opacidadVision = gameConfig->get_opacity();
        conoVision = gameConfig->get_cone_angle();
    }

    void reSizeWindow(int width, int height) {
        widthWindow = width;
        heightWindow = height;

        if (width > 1200 || height > 1200) {
            zoom = 20.0f;
        } else {
            zoom = 15.0f;
        }
    }
};
#endif
