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

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            quit = true;
    }

    // Leer estado del teclado
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D]) {
        mensajes.push("wd");
        return;
    }
    if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A]) {
        mensajes.push("wa");
        return;
    }
    if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D]) {
        mensajes.push("sd");
        return;
    }
    if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_A]) {
        mensajes.push("sa");
        return;
    }

    if (state[SDL_SCANCODE_W]) mensajes.push("w");
    if (state[SDL_SCANCODE_A]) mensajes.push("a");
    if (state[SDL_SCANCODE_S]) mensajes.push("s");
    if (state[SDL_SCANCODE_D]) mensajes.push("d");
    if (state[SDL_SCANCODE_Q]) mensajes.push("q");
}

std::optional<std::string> InputHandler::getMensaje() {
    if (!mensajes.empty()) {
        std::string mensaje = mensajes.front();
        mensajes.pop();
        return mensaje;
    }
    return std::nullopt;
}
