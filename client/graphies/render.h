#ifndef RENDER_H
#define RENDER_H
#include <SDL.h>
#include "../../common/protocol.h"
#include "textureStore.h"

using namespace SDL2pp;

class Render{
    private:
    Renderer* sdlRenderer;
    TextureStore texturas;
    Protocol& protocolo;
    
    int mapa_x = 0, mapa_y = 0; // position of the map
	float posJugadorX = 0.0, posJugadorY = 0.0;
    double angle = 0.0;
	int mouseX = 0, mouseY = 0; // mouse position
	unsigned int prev_ticks = SDL_GetTicks();

    // Function to calculate the angle between the player and the mouse
    double getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey);

    public:

    Render(Renderer* renderer, Protocol& protocolo);
    void renderFrame(int posX, int posY);
    
};
#endif
