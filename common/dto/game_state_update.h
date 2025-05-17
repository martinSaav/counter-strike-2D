#ifndef GAME_STATE_UPDATE_H
#define GAME_STATE_UPDATE_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../action.h"
#include "../message.h"
#include "../message_type.h"


class GameStateUpdate: public Message {
private:
    MessageType message_type = MessageType::GameStateUpdate;
    uint8_t x;
    uint8_t y;

public:
    GameStateUpdate(uint8_t x, uint8_t y): x(x), y(y) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t payload_length = htons(2);
        memccpy(buffer + 1, &payload_length, 0, sizeof(payload_length));
        buffer[3] = x;
        buffer[4] = y;
    }

    size_t serialized_size() const override { return 5; }

    uint8_t get_x() const { return x; }

    uint8_t get_y() const { return y; }

    static GameStateUpdate deserialize(const uint8_t* buffer, size_t size) {
        if (size < 5) {
            throw std::runtime_error("");
        }
        uint8_t x_deserialized = buffer[3];
        uint8_t y_deserialized = buffer[4];
        return GameStateUpdate(x_deserialized, y_deserialized);
    }

    MessageType type() const override { return this->message_type; }
};
#endif
