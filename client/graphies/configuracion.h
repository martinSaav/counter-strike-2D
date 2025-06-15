#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <SDL.h>

struct Configuracion{
    SDL_Rect camera = {0, 0, 0, 0};
    float zoom = 20.0f;
    int widthWindow, heightWindow;
    int tiempoDeCompra = 10; //45;

    // Constructor
    Configuracion(int width, int height) {
        widthWindow = width;
        heightWindow = height;

        int camWidth = widthWindow / zoom;
        int camHeight = heightWindow / zoom;
        camera = {0, 0, camWidth, camHeight};
    }
};
#endif
