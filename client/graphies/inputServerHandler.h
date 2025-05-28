#define INPUTSERVERHANDLER_H
#ifdef INPUTSERVERHANDLER_H
#include <mutex>
#include <optional>
#include <queue>

#include "../../common/dto/game_state_update.h"
#include "../../common/protocol.h"

// Recibe mensajes del servidor y los guarda en una cola
class InputServerHandler {
private:
    bool quit = false;
    std::queue<GameStateUpdate> mensajes;
    std::mutex mtx;
    Protocol& protocolo;

public:
    explicit InputServerHandler(Protocol& protocolo);
    ~InputServerHandler();

    void processEvents();

    std::optional<GameStateUpdate> getMensaje();

    bool exitGame();
};
#endif
