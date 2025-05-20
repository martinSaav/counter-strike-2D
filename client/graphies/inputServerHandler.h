#define INPUTSERVERHANDLER_H
#ifdef INPUTSERVERHANDLER_H
#include <string>
#include <queue>
#include <optional>
#include <mutex>
#include "../../common/protocol.h"
#include "../../common/dto/game_state_update.h"

// Recibe mensajes del servidor y los guarda en una cola
class InputServerHandler {
    private:
    bool quit = false;
    std::queue<GameStateUpdate> mensajes;
    std::mutex mtx;
    Protocol& protocolo;

    public:
    InputServerHandler(Protocol& protocolo);
    ~InputServerHandler();  

    void processEvents();

    std::optional<GameStateUpdate> getMensaje();

    bool exitGame();
};
#endif
