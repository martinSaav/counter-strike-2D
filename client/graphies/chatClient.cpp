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

ChatClient::ChatClient(Protocol& protocolo, std::string& namePlayer):
        protocolo(protocolo), namePlayer(namePlayer) {}

// Definición
double getCurrentTime() {
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    return (double)SDL_GetPerformanceCounter() / frequency;
}

int ChatClient::run(std::unique_ptr<GameStateUpdate>& estadistics) {
    SDL sdl(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_ShowCursor(SDL_DISABLE);  // desabilitamos el mouse


    // Recibimos la configuracion
    const std::unique_ptr<Message> gameConfigRequest = protocolo.recv_message();
    auto gameConfig = dynamic_cast<GameConfigInfo*>(gameConfigRequest.get());

    // Create main window
    int widthWindow = 1000;
    int heightWindow = 1000;

    Window window("Counter Strike 1.6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthWindow,
                  heightWindow, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);  // pantalla completa

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create render object
    Configuracion configuracion(widthWindow, heightWindow, gameConfig);

    const char* assetsPath = std::getenv("CS2D_ASSETS_DIR");
    if (!assetsPath) {
        assetsPath = "../client/data";
    }

    std::string assetsPathStr(assetsPath);

    Render render(&renderer, protocolo, namePlayer, configuracion, assetsPathStr);

    // Create input handler
    InputHandler inputHandler(protocolo, configuracion, gameOver, clientClosed);

    InputServerHandler inputServerHandler(protocolo, gameOver);

    std::thread inputThread(&InputHandler::processEvents, &inputHandler);

    std::thread inputServer(&InputServerHandler::processEvents, &inputServerHandler);


    const double FPS = 30.0;
    const double FRAME_TIME = 1.0 / FPS;  // en segundos
    // Main loop

    std::optional<GameStateUpdate> mensaje;
    while (!gameOver) {
        double current = getCurrentTime();

        render.clearScreen();

        mensaje = inputServerHandler.getMensaje();
        if (mensaje) {
            render.renderFrame(mensaje);
        }

        // Sleep si el frame fue más rápido de lo esperado
        double frameTime = getCurrentTime() - current;
        double delay = FRAME_TIME - frameTime;

        if (delay > 0) {
            SDL_Delay((Uint32)(delay * 1000.0));  // convertir a milisegundos

        } else if (delay < 0) {

            while (delay < 0) {
                mensaje = inputServerHandler.getMensaje();
                if (!mensaje) {
                    break;  // no hay más mensajes para descartar
                }
                delay += FRAME_TIME;
            }
        }
    }

    protocolo.kill();
    inputThread.join();
    inputServer.join();
    SDL_Quit();

    if (clientClosed) {
        return CLIENTCLOSED;
    }
    std::optional<GameStateUpdate> mensajeFinal = inputServerHandler.getMensaje();
    // Guardamos ultimo mensaje
    estadistics = std::make_unique<GameStateUpdate>(std::move(mensajeFinal.value()));
    return CONTINUAR;
}
