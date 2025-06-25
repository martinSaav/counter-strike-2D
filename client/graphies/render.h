#ifndef RENDER_H
#define RENDER_H
#include <iostream>
#include <memory>
#include <string>

#include "../../common/action.h"
#include "../../common/protocol.h"
#include "components/hudSprite.h"
#include "components/mapSprite.h"
#include "components/playerSprite.h"

#include "textureStore.h"

using SDL2pp::Renderer;
using SDL2pp::Texture;

struct BloodStain {
    int x;
    int y;
};

class Render {
private:
    Renderer* sdlRenderer;
    Protocol& protocolo;
    std::string& namePlayer;

    Configuracion& configuracion;

    HudSprite hud;
    MapSprite mapa;
    PlayerSprite player;

    // Coordenadas de mi jugador
    PlayerInfo* myPlayer = nullptr;

    // Angulo de mi jugador
    double myAngle = 0.0;

    // Angulo default
    double anglePlayer = 0.0;

    // Pos mouse
    int mouseX = 0, mouseY = 0;
    unsigned int prev_ticks = SDL_GetTicks();

    int damageFlashTimer = 0;
    int lastHealth = -1;

    std::vector<BloodStain> bloodStains;
    std::unordered_map<std::string, int> lastHealths;


    // Function to calculate the angle between the player and the mouse
    double getAnglePlayer(int jugadorX, int jugadorY, int mousex, int mousey);

    float normalizarAngulo(float angulo);

public:
    Render(Renderer* renderer, Protocol& protocolo, std::string& namePlayer,
           Configuracion& configuracion);
    void renderFrame(std::optional<GameStateUpdate> mensaje);
    bool puntoEnVision(int playerX, int playerY, float visionAngleDeg, int puntoX, int puntoY);
    void clearScreen();
};
#endif
