#include "inputHandler.h"

#include <SDL.h>

#include "../../common/action.h"
#include "../../common/dto/player_action.h"

InputHandler::InputHandler(Protocol& protocolo): protocolo(protocolo) {
    SDL_StartTextInput();  // Activa entrada de texto
}

InputHandler::~InputHandler() {
    SDL_StopTextInput();  // Limpia al salir
}

void InputHandler::processEvents() {
    SDL_Event event;


    while (!exitGame()) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
        }

        Action actionActual;


        Action* action = nullptr;

        // Leer estado del teclado
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_Q]) {

            quit = true;
        }

        if (state[SDL_SCANCODE_W]) {
            actionActual = Action::MoveUp;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_A]) {
            actionActual = Action::MoveLeft;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_S]) {
            actionActual = Action::MoveDown;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_D]) {
            actionActual = Action::MoveRight;
            action = &actionActual;

        } else if (state[SDL_BUTTON_LEFT]) {
            actionActual = Action::Shoot;
            action = &actionActual;
        }


        if (action) {
            PlayerAction playerAction(*action);
            protocolo.send_message(playerAction);
        }
        SDL_Delay(33);
    }
    abort();
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

bool InputHandler::exitGame() { return quit; }
