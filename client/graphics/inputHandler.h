#define INPUTHANDLER_H
#ifdef INPUTHANDLER_H

#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <string>

#include "../../common/action.h"
#include "../../common/dto/buy_ammo_request.h"
#include "../../common/dto/buy_weapon_request.h"
#include "../../common/dto/disconnect_request.h"
#include "../../common/dto/player_action.h"
#include "../../common/protocol.h"

#include "configuration.h"

// Lee el teclado y envía los mensajes al servidor
class InputHandler {
private:
    bool& gameOver;
    bool& clientClosed;
    std::queue<std::string> messages;
    std::mutex mtx;
    Protocol& protocol;
    Configuration& configuration;

    int mouseX = 0;
    int mouseY = 0;
    int mouse_map_x = 0;
    int mouse_map_y = 0;
    Action currentAction = {};
    Action* action = nullptr;

    void sendMessage(Action& currentAction);

public:
    InputHandler(Protocol& protocol, Configuration& configuration, bool& gameOver,
                 bool& clientClosed);
    ~InputHandler();

    void processEvents();

    std::optional<std::string> getMessage();
};
#endif
