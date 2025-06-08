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
    bool quit = false;
    std::queue<std::string> mensajes;
    std::mutex mtx;
    Protocol& protocolo;
    int mouseX = 0;
    int mouseY = 0;
    Configuracion& configuracion;

public:
    InputHandler(Protocol& protocolo, Configuracion& configuracion);
    ~InputHandler();

    void processEvents();

    std::optional<std::string> getMensaje();

    bool exitGame();
};
#endif
