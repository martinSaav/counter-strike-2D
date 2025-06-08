#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <SDL.h>

struct Configuracion{
    SDL_Rect camera = {0, 0, 0, 0};
    float zoom = 10.0f;
};
#endif
