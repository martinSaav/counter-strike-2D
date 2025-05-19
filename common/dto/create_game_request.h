#ifndef CREATE_GAME_REQUEST_H
#define CREATE_GAME_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class CreateGameRequest: public Message {
private:
    MessageType message_type = MessageType::CreateGameRequest;
    std::string game_name;

public:
    explicit CreateGameRequest(std::string game_name): game_name(std::move(game_name)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(game_name.size()));
        memcpy(buffer + 1, &length, sizeof(length));
        memcpy(buffer + 3, game_name.data(), game_name.size());
    }

    size_t serialized_size() const override { return 3 + game_name.size(); }

    const std::string& get_game_name() const { return game_name; }


    static CreateGameRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        size_t expected_size = 3 + static_cast<size_t>(length);
        if (size < expected_size) {
            throw std::runtime_error("");
        }
        const std::string game_name_deserialized(reinterpret_cast<const char*>(buffer + 3), length);
        return CreateGameRequest(game_name_deserialized);
    }

    MessageType type() const override { return this->message_type; }
};
#endif
