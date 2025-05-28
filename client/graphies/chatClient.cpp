#include "chatClient.h"

#include <thread>

#include "SDL_image.h"
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

void ChatClient::run() {
    SDL sdl(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_ShowCursor(SDL_DISABLE);  // desabilitamos el mouse

    // Create main window
    Window window("Counter Strike 1.6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000,
                  SDL_WINDOW_SHOWN);  // pantalla completa

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create render object
    Render render(&renderer, protocolo, namePlayer);

    // Create input handler
    InputHandler inputHandler(protocolo);

    InputServerHandler inputServerHandler(protocolo);

    std::thread inputThread(&InputHandler::processEvents, &inputHandler);

    std::thread inputServer(&InputServerHandler::processEvents, &inputServerHandler);


    const double FPS = 30.0;
    const double FRAME_TIME = 1.0 / FPS;  // en segundos

    double lastTime = getCurrentTime();
    // Main loop
    while (true) {
        double current = getCurrentTime();
        double elapsed = current - lastTime;
        lastTime = current;

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
    inputThread.join();
    inputServer.join();
    SDL_Quit();
}
