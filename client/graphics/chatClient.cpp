#include "chatClient.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "inputHandler.h"
#include "inputServerHandler.h"
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::Window;

namespace {

constexpr double FPS = 30.0;
constexpr double FRAME_TIME = 1.0 / FPS;  // en segundos
constexpr int WINDOW_SIZE = 1000;

double getCurrentTime() {
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    return (double)SDL_GetPerformanceCounter() / frequency;
}

// Las imagenes y sonidos: CS2D_ASSETS_DIR (lo define el instalador) o ../client/data al correr
// desde build/
std::string findAssetsPath() {
    const char* assetsPath = std::getenv("CS2D_ASSETS_DIR");
    return assetsPath ? assetsPath : "../client/data";
}

// Con CS2D_DEBUG_FRAMES definida imprime cada 5 s el tiempo medio de frame y cuantos estados
// del servidor se juntaron por frame (si crece, el cliente no da abasto).
class FrameStats {
    const bool enabled = std::getenv("CS2D_DEBUG_FRAMES") != nullptr;
    double start = getCurrentTime();
    double frameTime = 0;
    int frames = 0;
    int states = 0;

public:
    void addState() { states++; }

    void addFrame(double seconds) {
        if (!enabled) {
            return;
        }
        frameTime += seconds;
        frames++;
        if (getCurrentTime() - start >= 5.0) {
            std::cerr << "[frames] frame medio " << (frameTime / frames) * 1000
                      << " ms, estados por frame " << double(states) / frames << "\n";
            start = getCurrentTime();
            frameTime = 0;
            frames = 0;
            states = 0;
        }
    }
};

}  // namespace

ChatClient::ChatClient(Protocol& protocol, std::string& namePlayer):
        protocol(protocol), namePlayer(namePlayer) {}

int ChatClient::run(std::unique_ptr<GameStateUpdate>& statistics) {
    SDL sdl(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_ShowCursor(SDL_DISABLE);  // el cursor lo dibuja el HUD (la mira)

    // Lo primero que manda el servidor al arrancar la partida es la configuracion (mapa, armas,
    // tiempos)
    const std::unique_ptr<Message> gameConfigRequest = protocol.recv_message();
    auto gameConfig = dynamic_cast<GameConfigInfo*>(gameConfigRequest.get());

    Window window("Counter Strike 1.6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE,
                  WINDOW_SIZE, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    Configuration configuration(WINDOW_SIZE, WINDOW_SIZE, gameConfig);
    std::string assetsPath = findAssetsPath();
    Render render(&renderer, protocol, namePlayer, configuration, assetsPath);

    // La entrada se procesa en este hilo (el que creo la ventana); los estados del servidor
    // llegan por otro hilo que los encola
    InputHandler inputHandler(protocol, configuration, gameOver, clientClosed);
    InputServerHandler inputServerHandler(protocol, gameOver);
    std::thread inputServer(&InputServerHandler::processEvents, &inputServerHandler);

    std::optional<GameStateUpdate> lastState = gameLoop(render, inputHandler, inputServerHandler);

    protocol.kill();
    inputServer.join();
    SDL_Quit();

    return collectStatistics(inputServerHandler, std::move(lastState), statistics);
}

std::optional<GameStateUpdate> ChatClient::gameLoop(Render& render, InputHandler& inputHandler,
                                                    InputServerHandler& inputServerHandler) {
    // Loop a ritmo constante (FPS): entrada, ultimo estado del servidor, dibujo y espera
    FrameStats stats;
    std::optional<GameStateUpdate> message;
    while (!gameOver) {
        double current = getCurrentTime();

        bool isBuyStage = message && message->get_stage() == RoundStage::Buy;
        inputHandler.processFrame(isBuyStage);

        render.clearScreen();

        // Drenamos toda la cola y nos quedamos con el estado más nuevo. El server produce estados
        // al mismo ritmo que renderizamos (30 Hz), así que consumir uno solo por frame haría que
        // cualquier backlog acumulado en el arranque (mientras se cargaban texturas) nunca se
        // drene, dejando el render permanentemente atrasado respecto al estado real.
        std::optional<GameStateUpdate> latest;
        std::optional<GameStateUpdate> next;
        while ((next = inputServerHandler.getMessage())) {
            latest = std::move(next);
            stats.addState();
        }
        if (latest) {
            message = std::move(latest);
            render.renderFrame(message);
        }

        double frameTime = getCurrentTime() - current;
        stats.addFrame(frameTime);
        double delay = FRAME_TIME - frameTime;
        if (delay > 0) {
            SDL_Delay((Uint32)(delay * 1000.0));  // convertir a milisegundos
        }
    }
    return message;
}

int ChatClient::collectStatistics(InputServerHandler& inputServerHandler,
                                  std::optional<GameStateUpdate> lastState,
                                  std::unique_ptr<GameStateUpdate>& statistics) {
    if (clientClosed || inputServerHandler.isConnectionLost()) {
        return CLIENTCLOSED;
    }
    // El ultimo estado (con las estadisticas finales) puede seguir en la cola o haberlo
    // consumido ya el loop
    std::optional<GameStateUpdate> finalMessage = inputServerHandler.getMessage();
    if (!finalMessage) {
        finalMessage = std::move(lastState);
    }
    if (!finalMessage) {
        return CLIENTCLOSED;
    }
    statistics = std::make_unique<GameStateUpdate>(std::move(finalMessage.value()));
    return CONTINUE;
}
