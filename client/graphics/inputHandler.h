#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <iostream>
#include <optional>
#include <utility>

#include "../../common/action.h"
#include "../../common/dto/buy_ammo_request.h"
#include "../../common/dto/buy_weapon_request.h"
#include "../../common/dto/disconnect_request.h"
#include "../../common/dto/player_action.h"
#include "../../common/protocol.h"

#include "configuration.h"

// Lee el teclado y el mouse y envía los mensajes al servidor. Se llama una vez por frame desde
// el loop principal (SDL pide que los eventos se procesen en el hilo que creó la ventana).
class InputHandler {
private:
    Protocol& protocol;
    Configuration& configuration;
    bool& gameOver;
    bool& clientClosed;

    int mouseX = 0;
    int mouseY = 0;
    int mouse_map_x = 0;
    int mouse_map_y = 0;
    int lastAimMouseX = -1;
    int lastAimMouseY = -1;
    bool shopClicked = false;
    bool buyStage = false;
    Action currentAction = {};
    Action* action = nullptr;

    void sendMessage(Action& currentAction);
    void handleEvents(bool isBuyStage);
    void handleKeyboard();
    // Si hay una diagonal apretada: el movimiento vertical y el horizontal, en ese orden
    static std::optional<std::pair<Action, Action>> diagonalMovement(const Uint8* state);
    void quit();
    void buyWeapon(Weapon weapon);
    void buyAmmo(WeaponType weaponType);
    void toggleFullscreen(Uint32 windowID);
    bool clickShop(int x, int y);
    bool isOverShop(int x, int y) const;

public:
    InputHandler(Protocol& protocol, Configuration& configuration, bool& gameOver,
                 bool& clientClosed);
    ~InputHandler();

    void processFrame(bool isBuyStage);
};
#endif
