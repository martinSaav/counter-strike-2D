#ifndef RENDER_H
#define RENDER_H
#include <iostream>
#include <list>
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

// Lo que se calcula una vez por frame y usan todas las capas de dibujo
struct FrameContext {
    const GameStateUpdate& state;
    std::list<PlayerInfo> players;
    const PlayerInfo* me = nullptr;  // mi jugador, dentro de players
    double myAngle = 0.0;            // angulo hacia el mouse, en grados
    float visionAngle = 0.0f;        // el mismo angulo en [0, 360)
    bool isBuyStage = false;
};

class Render {
private:
    Renderer* sdlRenderer;
    Protocol& protocol;
    std::string& namePlayer;
    std::string& assetsPath;

    Configuration& configuration;

    // Recursos compartidos por los componentes (se declaran antes que ellos para que se
    // construyan primero)
    TextureStore textures;
    MusicManager sounds;

    HudSprite hud;
    MapSprite map;
    PlayerSprite player;

    // Posicion del mouse en la ventana
    int mouseX = 0, mouseY = 0;

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

    // Capas del frame, en el orden en que se dibujan (ver renderFrame)
    void updateCamera(const PlayerInfo& me);
    void updateAim(FrameContext& frame);
    void drawBomb(const FrameContext& frame);
    void drawPlayers(const FrameContext& frame);
    void playPlayerSounds(const FrameContext& frame, const PlayerInfo& playerInfo, Uint32 now);
    void drawHudLayer(const FrameContext& frame);
    void drawWorldOverlays(const FrameContext& frame);
    void drawFeedback(const FrameContext& frame);
    bool isVisible(const FrameContext& frame, int x, int y);

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
