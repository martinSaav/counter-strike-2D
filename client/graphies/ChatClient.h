#define CHATCLIENT_H
#ifdef CHATCLIENT_H
#include "../../common/protocol.h"

class ChatClient {
    private:
    Protocol& protocolo; // Protocolo a usar

    public:
    void run(); //loop 
    
    ChatClient(Protocol& protocolo); // Constructor
};
#endif
