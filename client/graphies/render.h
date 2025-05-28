#ifndef RENDER_H
#define RENDER_H
#include <string>

#include <SDL.h>

#include "../../common/dto/game_state_update.h"
#include "../../common/protocol.h"

#include "textureStore.h"

using SDL2pp::Renderer;
using SDL2pp::Texture;

class Render {
private:
    Renderer* sdlRenderer;
    TextureStore texturas;
    Protocol& protocolo;
    std::string& namePlayer;

    // Pos del mapa
    int mapa_x = 0, mapa_y = 0;
    // Coordenadas de mi jugador
    float posJugadorX = 0.0, posJugadorY = 0.0;
    // Angulo de mi jugador
    double angle = 0.0;
    // Pos mouse
    int mouseX = 0, mouseY = 0;
    unsigned int prev_ticks = SDL_GetTicks();

    // Function to calculate the angle between the player and the mouse
    double getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey);

public:
    Render(Renderer* renderer, Protocol& protocolo, std::string& namePlayer);
    void renderFrame(std::optional<GameStateUpdate> mensaje);
};
#endif
