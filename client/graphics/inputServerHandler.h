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
    bool& gameOver;
    std::queue<GameStateUpdate> messages;
    std::mutex mtx;
    Protocol& protocol;

public:
    explicit InputServerHandler(Protocol& protocol, bool& gameOver);
    ~InputServerHandler();

    void processEvents();

    std::optional<GameStateUpdate> getMessage();
};
#endif
