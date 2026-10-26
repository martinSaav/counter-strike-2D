#ifndef RENDER_H
#define RENDER_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

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
    Protocol& protocol;
    std::string& namePlayer;
    std::string& assetsPath;

    Configuration& configuration;

    HudSprite hud;
    MapSprite map;
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
    std::unordered_map<std::string, std::pair<int, int>> lastPositions;
    std::unordered_map<std::string, Uint32> lastStepTicks;
    int lastTaskProgress = 0;
    Uint32 lastBombBeep = 0;
    Uint32 lastEmptyClick = 0;
    Weapon lastPrimary = Weapon::None;
    bool lastHadBomb = false;
    bool wasAlive = false;

    // Sonidos que dependen del estado y no de un jugador en particular: pitido de la bomba,
    // gatillo sin balas y levantar/tirar armas
    void playStateSounds(const GameStateUpdate& message, const PlayerInfo& me, bool isBuyStage);


    // Function to calculate the angle between the player and the mouse
    double getAnglePlayer(int playerX, int playerY, int mousex, int mousey);

    float normalizeAngle(float angle);

public:
    Render(Renderer* renderer, Protocol& protocol, std::string& namePlayer,
           Configuration& configuration, std::string& assetsPath);
    void renderFrame(std::optional<GameStateUpdate> message);
    bool pointInVision(int playerX, int playerY, float visionAngleDeg, int puntoX, int puntoY);
    void clearScreen();
};
#endif
