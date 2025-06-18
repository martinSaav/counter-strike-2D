#include "inputHandler.h"

#include "../../common/action.h"
#include "../../common/dto/player_action.h"
#include "../../common/dto/disconnect_request.h"
#include "../../common/dto/buy_weapon_request.h"
#include "../../common/dto/buy_ammo_request.h"

InputHandler::InputHandler(Protocol& protocolo, Configuracion& configuracion, bool& gameOver, bool& clientClosed): 
protocolo(protocolo), configuracion(configuracion), gameOver(gameOver), clientClosed(clientClosed){
    SDL_StartTextInput();  // Activa entrada de texto
}

InputHandler::~InputHandler() {
    SDL_StopTextInput();  // Limpia al salir
}

void InputHandler::processEvents() {
    SDL_Event event;

    Weapon weapon = Weapon::None;
    WeaponType weaponType = WeaponType::Knife;
    Weapon lastWeapon = Weapon::None;
    while (!gameOver) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                gameOver = true;
                clientClosed = true;
                DisconnectRequest disconnect;
                protocolo.send_message(disconnect);
            } 
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    int newWidth = event.window.data1;
                    int newHeight = event.window.data2;

                    configuracion.heightWindow = newHeight;
                    configuracion.widthWindow = newWidth;
                }
            }
        }

        Action actionActual;
        Action* action = nullptr;

        // Leer estado del teclado
        const Uint8* state = SDL_GetKeyboardState(NULL);
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (state[SDL_SCANCODE_Q]) {

            gameOver = true;
            clientClosed = true;
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

        } else if (state[SDL_SCANCODE_4]) {
            actionActual = Action::SetBomb;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_R]) {
            actionActual = Action::Reload;
            action = &actionActual;

        } else if (state[SDL_SCANCODE_E]) {
            actionActual = Action::EquipWeapon;
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
            weapon = Weapon::AK47;

        } else if (state[SDL_SCANCODE_O]) {
            weapon = Weapon::M3;

        } else if (state[SDL_SCANCODE_P]) {
            weapon = Weapon::AWP;
        }

        if (weapon != Weapon::None && weapon != lastWeapon){
            BuyWeaponRequest buyWeapon(weapon);
            protocolo.send_message(buyWeapon);
            lastWeapon = weapon;
            weapon = Weapon::None;
        }

        if (state[SDL_SCANCODE_K]) {
            weaponType = WeaponType::Primary;

        } else if (state[SDL_SCANCODE_L]) {
            weaponType = WeaponType::Secondary;
        }

        if (weaponType != WeaponType::Knife){
            BuyAmmoRequest buyAmmo(weaponType);
            protocolo.send_message(buyAmmo);
            weaponType = WeaponType::Knife;
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
