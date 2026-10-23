#include "inputHandler.h"

InputHandler::InputHandler(Protocol& protocol, Configuration& configuration, bool& gameOver,
                           bool& clientClosed):
        protocol(protocol),
        configuration(configuration),
        gameOver(gameOver),
        clientClosed(clientClosed) {
    SDL_StartTextInput();  // Activa entrada de texto
}

InputHandler::~InputHandler() {
    SDL_StopTextInput();  // Limpia al salir
}

void InputHandler::processEvents() {
    try {
        handleEvents();
    } catch (const std::exception& e) {
        // El socket se cerró (lo cerramos nosotros o el servidor) mientras enviábamos
        if (!gameOver) {
            std::cerr << "Error sending to server: " << e.what() << "\n";
        }
        gameOver = true;
    }
}

void InputHandler::handleEvents() {
    SDL_Event event;

    Weapon weapon = Weapon::None;
    WeaponType weaponType = WeaponType::Knife;
    Weapon lastWeapon = Weapon::None;
    while (!gameOver) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameOver = true;
                clientClosed = true;
                DisconnectRequest disconnect;
                protocol.send_message(disconnect);
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    int newWidth = event.window.data1;
                    int newHeight = event.window.data2;

                    configuration.reSizeWindow(newWidth, newHeight);
                }
            }
        }

        action = nullptr;

        // Leer estado del teclado
        const Uint8* state = SDL_GetKeyboardState(NULL);
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (state[SDL_SCANCODE_Q]) {

            gameOver = true;
            clientClosed = true;
            DisconnectRequest disconnect;
            protocol.send_message(disconnect);
        }

        if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_D]) {
            currentAction = Action::MoveUp;
            sendMessage(currentAction);

            currentAction = Action::MoveRight;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_W] && state[SDL_SCANCODE_A]) {
            currentAction = Action::MoveUp;
            sendMessage(currentAction);

            currentAction = Action::MoveLeft;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_A]) {
            currentAction = Action::MoveDown;
            sendMessage(currentAction);

            currentAction = Action::MoveLeft;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_S] && state[SDL_SCANCODE_D]) {
            currentAction = Action::MoveDown;
            sendMessage(currentAction);

            currentAction = Action::MoveRight;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_W]) {
            currentAction = Action::MoveUp;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_A]) {
            currentAction = Action::MoveLeft;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_S]) {
            currentAction = Action::MoveDown;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_D]) {
            currentAction = Action::MoveRight;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_1]) {
            currentAction = Action::SetKnife;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_2]) {
            currentAction = Action::SetPrimaryWeapon;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_3]) {
            currentAction = Action::SetSecondaryWeapon;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_4]) {
            currentAction = Action::SetBomb;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_R]) {
            currentAction = Action::Reload;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_E]) {
            currentAction = Action::EquipWeapon;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_F]) {
            currentAction = Action::DefuseBomb;
            action = &currentAction;

        } else if (state[SDL_SCANCODE_G]) {
            currentAction = Action::DropWeapon;
            action = &currentAction;
        }

        if (action) {
            sendMessage(*action);
        }

        // Disparo (por click, independiente del movimiento)
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            currentAction = Action::Shoot;
            sendMessage(currentAction);
        }

        // Compra independiente
        if (state[SDL_SCANCODE_I]) {
            weapon = Weapon::AK47;

        } else if (state[SDL_SCANCODE_O]) {
            weapon = Weapon::M3;

        } else if (state[SDL_SCANCODE_P]) {
            weapon = Weapon::AWP;
        }

        if (weapon != Weapon::None && weapon != lastWeapon) {
            BuyWeaponRequest buyWeapon(weapon);
            protocol.send_message(buyWeapon);
            lastWeapon = weapon;
            weapon = Weapon::None;
        }

        if (state[SDL_SCANCODE_K]) {
            weaponType = WeaponType::Primary;

        } else if (state[SDL_SCANCODE_L]) {
            weaponType = WeaponType::Secondary;
        }

        if (weaponType != WeaponType::Knife) {
            BuyAmmoRequest buyAmmo(weaponType);
            protocol.send_message(buyAmmo);
            weaponType = WeaponType::Knife;
        }
        SDL_Delay(33);
    }
}

std::optional<std::string> InputHandler::getMessage() {
    std::lock_guard<std::mutex> lock(mtx);

    if (!messages.empty()) {
        std::string message = messages.front();
        messages.pop();
        return message;
    }
    return std::nullopt;
}

void InputHandler::sendMessage(Action& currentAction) {
    mouse_map_x = int(mouseX / configuration.zoom + configuration.camera.x);
    mouse_map_y = int(mouseY / configuration.zoom + configuration.camera.y);
    PlayerAction playerAction(currentAction, mouse_map_x, mouse_map_y);
    protocol.send_message(playerAction);
}
