#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <memory>
#include <stdexcept>

#include "../common/catedra/socket.h"
#include "../common/message.h"
#include "../common/message_type.h"

// Se lanza cuando el peer cerró la conexión antes de que llegue un mensaje completo.
struct ConnectionClosed: public std::runtime_error {
    ConnectionClosed(): std::runtime_error("Connection closed by peer") {}
};

// Manda y recibe mensajes enteros. Cada mensaje viaja como
//     tipo (1 byte) | largo del payload (uint16, orden de red) | payload
// El payload lo escribe cada mensaje (Message::write_payload) y lo lee message_registry.
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
