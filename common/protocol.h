#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <variant>
#include <memory>

#include "../common/common_socket.h"
#include "../common/message_type.h"
#include "../common/message.h"

#define HEADER_SIZE 3


class Protocol {
private:
    Socket& peer;

public:
    explicit Protocol(Socket& peer);
    std::unique_ptr<Message> recv_message();
    void send_message(const Message& response);
    void kill();
};
#endif