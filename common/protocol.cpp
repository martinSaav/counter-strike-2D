#include "protocol.h"


#include <cstring>
#include <stdexcept>
#include <utility>


#include "../common/dto/login_request.h"
#include "../common/dto/player_action.h"
#include <vector>



Protocol::Protocol(Socket& peer) : peer(peer) {}


std::unique_ptr<Message> Protocol::recv_message() {
    uint8_t header[HEADER_SIZE];
    peer.recvall(header, HEADER_SIZE);
    MessageType message_type = static_cast<MessageType>(header[0]);
    uint16_t payload_length;
    memcpy(&payload_length, header + 1, sizeof(payload_length));
    payload_length = ntohs(payload_length);
    std::vector<uint8_t> buffer(HEADER_SIZE + payload_length);

    buffer[0] = static_cast<uint8_t>(message_type);
    memcpy(buffer.data() + sizeof(message_type), &payload_length, sizeof(payload_length));


    if (payload_length > 0) {
        peer.recvall(buffer.data() + HEADER_SIZE, payload_length);
    }


    switch (message_type) {
        case MessageType::LoginRequest: {
            return std::make_unique<LoginRequest>(
                    LoginRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::PlayerAction: {
            return std::make_unique<PlayerAction>(
                    PlayerAction::deserialize(buffer.data(), buffer.size()));
        }
        default:
            throw std::runtime_error("Unknown message type");
    }
}