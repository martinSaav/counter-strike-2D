#include "protocol.h"

#include <vector>

#include "../common/catedra/liberror.h"
#include "serialization/byte_reader.h"
#include "serialization/byte_writer.h"

#include "message_registry.h"

Protocol::Protocol(SocketInterface& peer): peer(peer) {}

std::unique_ptr<Message> Protocol::recv_message() {
    std::vector<uint8_t> header(HEADER_SIZE);
    // recvall retorna 0 si el socket se cerró sin recibir ningún byte
    if (peer.recvall(header.data(), HEADER_SIZE) == 0) {
        throw ConnectionClosed();
    }
    ByteReader header_reader(header);
    const auto type = header_reader.enum_u8<MessageType>();
    const uint16_t payload_length = header_reader.u16();

    std::vector<uint8_t> payload(payload_length);
    if (payload_length > 0 && peer.recvall(payload.data(), payload_length) == 0) {
        throw ConnectionClosed();
    }
    ByteReader payload_reader(payload);
    return read_message(type, payload_reader);
}

void Protocol::send_message(const Message& message) {
    ByteWriter payload;
    message.write_payload(payload);

    ByteWriter frame;
    frame.enum_u8(message.type());
    frame.u16(ByteWriter::checked_u16(payload.data().size(), "payload"));
    frame.raw(payload.data());
    peer.sendall(frame.data().data(), frame.data().size());
}

void Protocol::kill() {
    // Si el peer ya cerró la conexión el shutdown puede fallar (ENOTCONN); igual cerramos el socket
    try {
        peer.shutdown(2);
    } catch (const LibError&) {}
    peer.close();
}
