#include <thread>
#include <SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "SDL_image.h"
#include "ChatClient.h"
#include "InputHandler.h"
#include "Render.h"

using namespace SDL2pp;

ChatClient::ChatClient(const std::string& hostname, const std::string& servname){
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
    Render render(&renderer);

    // Create input handler
    InputHandler inputHandler;

    // Main loop
    while (true) {
        inputHandler.processEvents();
        auto mensaje = inputHandler.getMensaje();
        if(mensaje && mensaje.value() == "q"){
            break; // Exit the loop if "q" is pressed
        }
        // Si hay mensaje, lo mostramos, sino, mostramos una cadena vacía
        render.renderFrame(mensaje ? mensaje.value() : "");
        
        SDL_Delay(33);
    }
    SDL_Quit();
}
