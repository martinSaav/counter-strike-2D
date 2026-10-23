#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

#include "../common/catedra/socket.h"
#include "../common/message.h"
#include "../common/message_type.h"

// Se lanza cuando el peer cerró la conexión antes de que llegue un mensaje completo.
struct ConnectionClosed: public std::runtime_error {
    ConnectionClosed(): std::runtime_error("Connection closed by peer") {}
};

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
