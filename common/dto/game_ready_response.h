#ifndef GAME_READY_RESPONSE_H
#define GAME_READY_RESPONSE_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class GameReadyResponse: public Message {
private:
    MessageType message_type = MessageType::GameReadyResponse;
    bool success;

public:
    explicit GameReadyResponse(bool success): success(std::move(success)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(1));
        memcpy(buffer + 1, &length, sizeof(length));
        buffer[3] = static_cast<uint8_t>(success);
    }

    size_t serialized_size() const override { return 4; }

    const bool& get_success() const { return success; }


    static GameReadyResponse deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        size_t expected_size = 3 + static_cast<size_t>(length);
        if (size < expected_size) {
            throw std::runtime_error("");
        }
        const bool success_deserialized = static_cast<bool>(buffer[3]);
        return GameReadyResponse(success_deserialized);
    }

    MessageType type() const override { return this->message_type; }
};
#endif
