#ifndef JOIN_GAME_REQUEST_H
#define JOIN_GAME_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class JoinGameRequest: public Message {
private:
    MessageType message_type = MessageType::JoinGameRequest;
    std::string game_name;

public:
    explicit JoinGameRequest(std::string game_name): game_name(std::move(game_name)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(game_name.size()));
        memcpy(buffer + 1, &length, sizeof(length));
        memcpy(buffer + 3, game_name.data(), game_name.size());
    }

    static JoinGameRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        const std::string game_name_deserialized(reinterpret_cast<const char*>(buffer + 3), length);
        return JoinGameRequest(game_name_deserialized);
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override { return HEADER_SIZE + game_name.size(); }

    const std::string& get_game_name() const { return game_name; }
};
#endif
