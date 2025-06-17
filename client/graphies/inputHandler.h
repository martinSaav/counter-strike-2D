#define INPUTHANDLER_H
#ifdef INPUTHANDLER_H

#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <iostream>
#include "../../common/protocol.h"
#include "configuracion.h"

// Lee el teclado y envía los mensajes al servidor
class InputHandler {
private:
    bool& gameOver;
    bool& clientClosed;
    std::queue<std::string> mensajes;
    std::mutex mtx;
    Protocol& protocolo;
    Configuracion& configuracion;

    int mouseX = 0;
    int mouseY = 0;
    int mouse_map_x = 0;
    int mouse_map_y = 0;

public:
    InputHandler(Protocol& protocolo, Configuracion& configuracion, bool& gameOver, bool& clientClosed);
    ~InputHandler();

    void processEvents();

    std::optional<std::string> getMensaje();
};
#endif
