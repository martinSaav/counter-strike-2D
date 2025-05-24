#define INPUTHANDLER_H
#ifdef INPUTHANDLER_H
#include <string>
#include <queue>
#include <optional>
#include <mutex>
#include "../../common/protocol.h"

//Lee el teclado y envía los mensajes al servidor
class InputHandler {
    private:
    bool quit = false;
    std::queue<std::string> mensajes;
    std::mutex mtx;
    Protocol& protocolo;

    public:
    InputHandler(Protocol& protocolo);
    ~InputHandler();  

    void processEvents();

    std::optional<std::string> getMensaje();

    bool exitGame();
};
#endif
