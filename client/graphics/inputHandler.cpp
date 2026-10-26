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

void InputHandler::processFrame(bool isBuyStage) {
    buyStage = isBuyStage;
    try {
        handleEvents(isBuyStage);
        handleKeyboard();
    } catch (const std::exception& e) {
        // El socket se cerró (lo cerramos nosotros o el servidor) mientras enviábamos
        if (!gameOver) {
            std::cerr << "Error sending to server: " << e.what() << "\n";
        }
        gameOver = true;
    }
}

void InputHandler::quit() {
    gameOver = true;
    clientClosed = true;
    DisconnectRequest disconnect;
    protocol.send_message(disconnect);
}

void InputHandler::buyWeapon(Weapon weapon) {
    BuyWeaponRequest buyWeapon(weapon);
    protocol.send_message(buyWeapon);
}

void InputHandler::buyAmmo(WeaponType weaponType) {
    BuyAmmoRequest buyAmmo(weaponType);
    protocol.send_message(buyAmmo);
}

void InputHandler::toggleFullscreen(Uint32 windowID) {
    SDL_Window* window = SDL_GetWindowFromID(windowID);
    if (!window) {
        return;
    }
    bool isFullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_SetWindowFullscreen(window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool InputHandler::isOverShop(int x, int y) const {
    SDL_Rect shop = configuration.shopRect();
    return x >= shop.x && y >= shop.y && x < shop.x + shop.w && y < shop.y + shop.h;
}

bool InputHandler::clickShop(int x, int y) {
    // La imagen de la tienda mide 400x400; las filas de las armas y la municion estan en
    // posiciones fijas dentro de ella.
    if (!isOverShop(x, y)) {
        return false;
    }
    SDL_Rect shop = configuration.shopRect();
    float imageX = (x - shop.x) * 400.0f / shop.w;
    float imageY = (y - shop.y) * 400.0f / shop.h;

    if (imageY >= 60 && imageY < 150) {
        buyWeapon(Weapon::AK47);
    } else if (imageY >= 150 && imageY < 236) {
        buyWeapon(Weapon::M3);
    } else if (imageY >= 236 && imageY < 320) {
        buyWeapon(Weapon::AWP);
    } else if (imageY >= 320 && imageY < 380) {
        buyAmmo(imageX < 200 ? WeaponType::Primary : WeaponType::Secondary);
    }
    return true;
}

void InputHandler::handleEvents(bool isBuyStage) {
    SDL_Event event;
    shopClicked = false;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit();
            return;
        }
        if (event.type == SDL_WINDOWEVENT && (event.window.event == SDL_WINDOWEVENT_RESIZED ||
                                              event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)) {
            configuration.reSizeWindow(event.window.data1, event.window.data2);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT &&
            isBuyStage) {
            shopClicked = clickShop(event.button.x, event.button.y);
        }
        // Las compras y los atajos van por evento (una vez por pulsación), no por estado del
        // teclado, para no mandar una compra por frame mientras la tecla sigue apretada.
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
            switch (event.key.keysym.sym) {
                case SDLK_i:
                    buyWeapon(Weapon::AK47);
                    break;
                case SDLK_o:
                    buyWeapon(Weapon::M3);
                    break;
                case SDLK_p:
                    buyWeapon(Weapon::AWP);
                    break;
                case SDLK_k:
                    buyAmmo(WeaponType::Primary);
                    break;
                case SDLK_l:
                    buyAmmo(WeaponType::Secondary);
                    break;
                case SDLK_h:
                    configuration.toggleHelp();
                    break;
                case SDLK_F11:
                    toggleFullscreen(event.key.windowID);
                    break;
                default:
                    break;
            }
        }
    }
}

void InputHandler::handleKeyboard() {
    if (gameOver) {
        return;
    }
    action = nullptr;
    bool actionSent = false;

    // Leer estado del teclado
    const Uint8* state = SDL_GetKeyboardState(NULL);
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

    if (state[SDL_SCANCODE_Q]) {
        quit();
        return;
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
        actionSent = true;
    }

    // Disparo (por click, independiente del movimiento). Un click sobre la tienda es una compra.
    bool aimingAtShop = buyStage && isOverShop(mouseX, mouseY);
    if ((mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) && !shopClicked && !aimingAtShop) {
        currentAction = Action::Shoot;
        sendMessage(currentAction);
        actionSent = true;
    }

    // Si no hizo nada pero movio el mouse, avisamos hacia donde apunta para que los demas lo
    // vean girar.
    if (!actionSent && (mouseX != lastAimMouseX || mouseY != lastAimMouseY)) {
        currentAction = Action::Idle;
        sendMessage(currentAction);
    }
}

void InputHandler::sendMessage(Action& currentAction) {
    mouse_map_x = int(mouseX / configuration.zoom + configuration.camera.x);
    mouse_map_y = int(mouseY / configuration.zoom + configuration.camera.y);
    lastAimMouseX = mouseX;
    lastAimMouseY = mouseY;
    PlayerAction playerAction(currentAction, mouse_map_x, mouse_map_y);
    protocol.send_message(playerAction);
}
