#define CHATCLIENT_H
#ifdef CHATCLIENT_H
#include <string>

#include "render.h"

class ChatClient {
private:
    Protocol& protocolo;  // Protocolo a usar
    std::string& namePlayer;

    bool gameOver = false;

public:
    void run();  // loop

    ChatClient(Protocol& protocolo, std::string& namePlayer);  // Constructor
};
#endif
