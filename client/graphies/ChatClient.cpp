#include <thread>
#include "ChatClient.h"
#include "inputServerHandler.h"
using namespace SDL2pp;

ChatClient::ChatClient(Protocol& protocolo) : protocolo(protocolo) {
    // Constructor
}

void ChatClient::run(){
    SDL sdl(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_ShowCursor(SDL_DISABLE); //desabilitamos el mouse
	
	// Create main window
	Window window("Counter Strike 1.6",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			1000, 1000,
			SDL_WINDOW_SHOWN); //pantalla completa

	// Create accelerated video renderer with default driver
	Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create render object
    Render render(&renderer, protocolo);

    // Create input handler
    InputHandler inputHandler(protocolo);

    InputServerHandler inputServerHandler(protocolo);

    std::thread inputThread(&InputHandler::processEvents, &inputHandler);

    std::thread inputServer(&InputServerHandler::processEvents, &inputServerHandler);

    // Main loop
    while (true) {
        
        auto mensaje = inputServerHandler.getMensaje();
        if(mensaje){
            render.renderFrame(mensaje->get_x(), mensaje->get_y());
        }
        // Si hay mensaje, lo mostramos, sino, mostramos una cadena vacía
        
        SDL_Delay(33);
    }
    inputThread.join();
    inputServer.join();
    SDL_Quit();
}
