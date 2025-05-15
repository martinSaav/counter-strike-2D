#ifndef RENDER_H
#define RENDER_H
#include <SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "SDL_image.h"

using namespace SDL2pp;

class Render{
    private:
    Renderer* sdlRenderer;
    Texture fondo;
    Texture mira;
    Texture sprites;
    
    int mapa_x = 0, mapa_y = 0; // position of the map
	float posJugadorX = 0.0, posJugadorY = 0.0;
    double angle = 0.0;
	int mouseX = 0, mouseY = 0; // mouse position
	unsigned int prev_ticks = SDL_GetTicks();

    public:

    Render(Renderer* renderer);
    void renderFrame(std::string mensaje);
    
};
#endif
