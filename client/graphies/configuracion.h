#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <SDL.h>

#include "../../common/dto/game_config_info.h"

struct Configuracion {

    GameConfigInfo* gameConfig;
    float zoom;
    int widthWindow, heightWindow;
    SDL_Rect camera;
    float tiempoDeCompra;
    int opacidadVision;
    int conoVision;

    Configuracion(int width, int height, GameConfigInfo* gameConfig):
            gameConfig(gameConfig),
            zoom(15.0f),
            widthWindow(width),
            heightWindow(height),
            camera{0, 0, int(width / zoom), int(height / zoom)},
            tiempoDeCompra(gameConfig->get_buy_time() / 1000.0f),
            opacidadVision(gameConfig->get_opacity()),
            conoVision(gameConfig->get_cone_angle()) {}


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
