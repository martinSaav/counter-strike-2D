#include "inputHandler.h"

#include "../../common/action.h"
#include "../../common/dto/player_action.h"
#include "../../common/dto/disconnect_request.h"
#include "../../common/dto/buy_weapon_request.h"

InputHandler::InputHandler(Protocol& protocolo, Configuracion& configuracion, bool& gameOver): 
protocolo(protocolo), configuracion(configuracion), gameOver(gameOver){
    SDL_StartTextInput();  // Activa entrada de texto
}

InputHandler::~InputHandler() {
    SDL_StopTextInput();  // Limpia al salir
}

void InputHandler::processEvents() {
    SDL_Event event;

    Weapon weapon = Weapon::None;

    while (!gameOver) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                gameOver = true;
        }

        Action actionActual;
        Action* action = nullptr;

        // Leer estado del teclado
        const Uint8* state = SDL_GetKeyboardState(NULL);
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (state[SDL_SCANCODE_Q]) {

            gameOver = true;
            DisconnectRequest disconnect;
            protocolo.send_message(disconnect);
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

        } else if (state[SDL_SCANCODE_1]) {
            actionActual = Action::SetKnife;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_2]) {
            actionActual = Action::SetPrimaryWeapon;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_3]) {
            actionActual = Action::SetSecondaryWeapon;
            action = &actionActual;

        }

        if (action) {
            mouse_map_x = int(mouseX / configuracion.zoom + configuracion.camera.x);
            mouse_map_y = int(mouseY / configuracion.zoom + configuracion.camera.y);
            PlayerAction playerAction(*action, mouse_map_x, mouse_map_y);
            protocolo.send_message(playerAction);
        }

        // Disparo (por click, independiente del movimiento)
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            mouse_map_x = int(mouseX / configuracion.zoom + configuracion.camera.x);
            mouse_map_y = int(mouseY / configuracion.zoom + configuracion.camera.y);
            protocolo.send_message(PlayerAction(Action::Shoot, mouse_map_x, mouse_map_y));
        }

        // Compra independiente
        if (state[SDL_SCANCODE_I]) {
            Weapon weapon = Weapon::AK47;

        } else if (state[SDL_SCANCODE_O]) {
            Weapon weapon = Weapon::M3;

        } else if (state[SDL_SCANCODE_P]) {
            Weapon weapon = Weapon::AWP;
        }

        if (weapon != Weapon::None){
            BuyWeaponRequest buyWeapon(weapon);
            protocolo.send_message(buyWeapon);
            weapon = Weapon::None;
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
