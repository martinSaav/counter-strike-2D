#include "chatClient.h"

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

ChatClient::ChatClient(Protocol& protocol, std::string& namePlayer):
        protocol(protocol), namePlayer(namePlayer) {}

// Definición
double getCurrentTime() {
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    return (double)SDL_GetPerformanceCounter() / frequency;
}

int ChatClient::run(std::unique_ptr<GameStateUpdate>& statistics) {
    SDL sdl(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_ShowCursor(SDL_DISABLE);  // desabilitamos el mouse


    // Recibimos la configuracion
    const std::unique_ptr<Message> gameConfigRequest = protocol.recv_message();
    auto gameConfig = dynamic_cast<GameConfigInfo*>(gameConfigRequest.get());

    // Create main window
    int widthWindow = 1000;
    int heightWindow = 1000;

    Window window("Counter Strike 1.6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthWindow,
                  heightWindow, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);  // pantalla completa

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create render object
    Configuration configuration(widthWindow, heightWindow, gameConfig);

    const char* assetsPath = std::getenv("CS2D_ASSETS_DIR");
    if (!assetsPath) {
        assetsPath = "../client/data";
    }

    std::string assetsPathStr(assetsPath);

    Render render(&renderer, protocol, namePlayer, configuration, assetsPathStr);

    // Create input handler
    InputHandler inputHandler(protocol, configuration, gameOver, clientClosed);

    InputServerHandler inputServerHandler(protocol, gameOver);

    std::thread inputThread(&InputHandler::processEvents, &inputHandler);

    std::thread inputServer(&InputServerHandler::processEvents, &inputServerHandler);


    const double FPS = 30.0;
    const double FRAME_TIME = 1.0 / FPS;  // en segundos
    // Main loop

    std::optional<GameStateUpdate> message;
    while (!gameOver) {
        double current = getCurrentTime();

        render.clearScreen();

        // Drenamos toda la cola y nos quedamos con el estado más nuevo. El server produce estados al
        // mismo ritmo que renderizamos (30 Hz), así que consumir uno solo por frame haría que
        // cualquier backlog acumulado en el arranque (mientras se cargaban texturas) nunca se drene,
        // dejando el render permanentemente atrasado respecto al estado real.
        std::optional<GameStateUpdate> ultimo;
        std::optional<GameStateUpdate> siguiente;
        while ((siguiente = inputServerHandler.getMessage())) {
            ultimo = std::move(siguiente);
        }
        if (ultimo) {
            message = std::move(ultimo);
            render.renderFrame(message);
        }

        // Sleep si el frame fue más rápido de lo esperado
        double frameTime = getCurrentTime() - current;
        double delay = FRAME_TIME - frameTime;

        if (delay > 0) {
            SDL_Delay((Uint32)(delay * 1000.0));  // convertir a milisegundos
        }
    }

    protocol.kill();
    inputThread.join();
    inputServer.join();
    SDL_Quit();

    if (clientClosed || inputServerHandler.isConnectionLost()) {
        return CLIENTCLOSED;
    }
    std::optional<GameStateUpdate> finalMessage = inputServerHandler.getMessage();
    // El main loop pudo haber drenado ya el último estado de la cola
    if (!finalMessage) {
        finalMessage = std::move(message);
    }
    if (!finalMessage) {
        return CLIENTCLOSED;
    }
    // Guardamos ultimo mensaje
    statistics = std::make_unique<GameStateUpdate>(std::move(finalMessage.value()));
    return CONTINUE;
}
