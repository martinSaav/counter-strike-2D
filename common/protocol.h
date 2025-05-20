#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <memory>
#include <string>
#include <variant>

#include "../common/catedra/socket.h"
#include "../common/message.h"
#include "../common/message_type.h"

#define HEADER_SIZE 3


class Protocol {
private:
    SocketInterface& peer;

public:
    explicit Protocol(SocketInterface& peer);
    std::unique_ptr<Message> recv_message();
    void send_message(const Message& message);
    void kill();
};
#endif
