#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <variant>

#include "../common/common_socket.h"



class Protocol {
private:
    Socket& peer;

public:
    explicit Protocol(Socket& peer);
    void recv_message();
    void send_message(const int& response);
    void kill();
};
#endif