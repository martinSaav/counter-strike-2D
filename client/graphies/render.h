#ifndef RENDER_H
#define RENDER_H
#include <string>
#include <memory>

#include "../../common/dto/game_state_update.h"
#include "../../common/protocol.h"
#include "../../common/action.h"
#include "textureStore.h"
#include "configuracion.h"

#include "components/hudSprite.h"
#include "components/mapSprite.h"
#include "components/playerSprite.h"

using SDL2pp::Renderer;
using SDL2pp::Texture;

class Render {
private:
    Renderer* sdlRenderer;
    Protocol& protocolo;
    std::string& namePlayer;

    HudSprite hud;
    MapSprite mapa;
    PlayerSprite player;

    Configuracion& configuracion;

    int screenWidth = 1000;
    int screenHeight = 1000;

    // Pos del mapa
    int mapa_x = 0, mapa_y = 0;

    // Coordenadas de mi jugador
    PlayerInfo* myPlayer = nullptr;

    // Angulo de mi jugador
    double myAngle = 0.0;
    // Pos mouse
    int mouseX = 0, mouseY = 0;
    unsigned int prev_ticks = SDL_GetTicks();

    // Function to calculate the angle between the player and the mouse
    double getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey);

public:
    Render(Renderer* renderer, Protocol& protocolo, std::string& namePlayer, Configuracion& configuracion);
    void renderFrame(std::optional<GameStateUpdate> mensaje);
};
#endif
