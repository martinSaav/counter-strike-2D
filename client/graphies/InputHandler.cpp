#include "InputHandler.h"
#include <SDL.h>

InputHandler::InputHandler() {
    SDL_StartTextInput(); // Activa entrada de texto
}

InputHandler::~InputHandler() {
    SDL_StopTextInput(); // Limpia al salir
}

void InputHandler::processEvents() {
    SDL_Event event;
    std::string mensajeActual = "";

    while (!exitGame()){
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }
    
        // Leer estado del teclado
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_Q]) {
            mensajeActual = "q";
            quit = true;

        }else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D]) {
            mensajeActual = "wd";

        }else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A]) {
            mensajeActual = "wa";

        }else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D]) {
            mensajeActual = "sd";

        }else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_A]) {
            mensajeActual = "sa";

        }else if (state[SDL_SCANCODE_W]){
            mensajeActual = "w";

        }else if (state[SDL_SCANCODE_A]){
            mensajeActual = "a";

        }else if (state[SDL_SCANCODE_S]){
            mensajeActual = "s";

        }else if (state[SDL_SCANCODE_D]){
            mensajeActual = "d";
            
        }else if (state[SDL_SCANCODE_Q]){
            mensajeActual = "q";
        }

        if (mensajeActual != "") {
            std::lock_guard<std::mutex> lock(mtx);
            mensajes.push(mensajeActual);
            mensajeActual = ""; // Reinicia el mensaje actual
        }
    
        SDL_Delay(33);
    }
}

std::optional<std::string> InputHandler::getMensaje() {
    std::lock_guard<std::mutex> lock(mtx);

    if (!mensajes.empty()) {
        std::string mensaje = mensajes.front();
        mensajes.pop();
        return mensaje;
    }
    return std::nullopt;
}

bool InputHandler::exitGame(){
    return quit;
}