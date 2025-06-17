#include "chatClient.h"

#include <thread>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "inputHandler.h"
#include "inputServerHandler.h"
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::Window;

ChatClient::ChatClient(Protocol& protocolo, std::string& namePlayer):
        protocolo(protocolo), namePlayer(namePlayer) {
    // Constructor
}

// Definición
double getCurrentTime() {
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    return (double)SDL_GetPerformanceCounter() / frequency;
}

int ChatClient::run(std::unique_ptr<GameStateUpdate>& estadistics) {
    SDL sdl(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_ShowCursor(SDL_DISABLE);  // desabilitamos el mouse

    // Create main window
    int widthWindow = 1000;
    int heightWindow = 1000;

    Window window("Counter Strike 1.6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        widthWindow, heightWindow, SDL_WINDOW_SHOWN);  // pantalla completa

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create render object
    Configuracion configuracion(widthWindow, heightWindow);
    
    Render render(&renderer, protocolo, namePlayer, configuracion);

    // Create input handler
    InputHandler inputHandler(protocolo, configuracion, gameOver, clientClosed);

    InputServerHandler inputServerHandler(protocolo, gameOver);

    std::thread inputThread(&InputHandler::processEvents, &inputHandler);

    std::thread inputServer(&InputServerHandler::processEvents, &inputServerHandler);


    const double FPS = 30.0;
    const double FRAME_TIME = 1.0 / FPS;  // en segundos

    // double lastTime = getCurrentTime();
    // Main loop
    while (!gameOver) {
        double current = getCurrentTime();
        // double elapsed = current - lastTime;
        // lastTime = current;

        auto mensaje = inputServerHandler.getMensaje();
        if (mensaje) {
            render.renderFrame(mensaje);
        }

        // Sleep si el frame fue más rápido de lo esperado
        double frameTime = getCurrentTime() - current;
        double delay = FRAME_TIME - frameTime;
        if (delay > 0) {
            SDL_Delay((Uint32)(delay * 1000.0));  // convertir a milisegundos
        }
        // SDL_Delay(33);
    }
    protocolo.kill();
    inputThread.join();
    inputServer.join();
    SDL_Quit();

    if (clientClosed){
        return CLIENTCLOSED;
    }
    std::optional<GameStateUpdate> mensaje = inputServerHandler.getMensaje();
    // Guardamos ultimo mensaje
    estadistics = std::make_unique<GameStateUpdate>(std::move(mensaje.value()));
    return CONTINUAR;
}
