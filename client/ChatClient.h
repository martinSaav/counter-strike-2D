#define CHATCLIENT_H
#ifdef CHATCLIENT_H

class ChatClient {
    private:
    // Protocolo

    public:
    void run(); //loop 
    
    ChatClient(const std::string& hostname, const std::string& servname); // Constructor
};
#endif
