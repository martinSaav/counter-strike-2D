#define CHATCLIENT_H
#ifdef CHATCLIENT_H
#include <memory>
#include <string>

#include "../../common/dto/game_config_info.h"

#include "render.h"
const int CONTINUAR = 1;
const int CLIENTCLOSED = 2;

class ChatClient {
private:
    Protocol& protocolo;  // Protocolo a usar
    std::string& namePlayer;
    bool clientClosed = false;
    bool gameOver = false;

public:
    int run(std::unique_ptr<GameStateUpdate>& estadistics);  // loop

    ChatClient(Protocol& protocolo, std::string& namePlayer);  // Constructor
};
#endif
