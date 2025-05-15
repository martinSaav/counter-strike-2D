#ifndef GAME_STATE_UPDATE_H
#define GAME_STATE_UPDATE_H

#include "../message_type.h"
#include "../message.h"
#include "../action.h"
#include <cstring>
#include <string>
#include <utility>
#include <netinet/in.h>
#include <stdexcept>
#include <cstdint>


class GameStateUpdate: public Message {
private:
    MessageType message_type = MessageType::GameStateUpdate;
    uint8_t x;
    uint8_t y;
public:
    GameStateUpdate(uint8_t x, uint8_t y)
        : x(x), y(y) {}
        
    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        buffer[1] = x;
        buffer[2] = y;
    }

    size_t serialized_size() const override {
        return 3;
    }

    uint8_t get_x() const {
        return x;
    }

    uint8_t get_y() const {
        return y;
    }

    static GameStateUpdate deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint8_t x_deserialized = buffer[1];
        uint8_t y_deserialized = buffer[2];
        return GameStateUpdate(x_deserialized, y_deserialized);
    }
};
#endif 
