#ifndef PLAYER_ACTION_H
#define PLAYER_ACTION_H

#include "../message_type.h"
#include "../message.h"
#include "../action.h"
#include <cstring>
#include <string>
#include <utility>
#include <netinet/in.h>
#include <stdexcept>
#include <cstdint>


class PlayerAction: public Message {
private:
    MessageType message_type = MessageType::PlayerAction;
    Action action;
public:
    PlayerAction(Action action)
        : action(action) {}
    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        buffer[1] = static_cast<uint8_t>(action);
    }

    size_t serialized_size() const override {
        return 4;
    }

    uint8_t get_action() const {
        return static_cast<uint8_t>(action);
    }

    static PlayerAction deserialize(const uint8_t* buffer, size_t size) {
        if (size < 4) {
            throw std::runtime_error(
                    "Buffer size is too small to deserialize PlayerAction");
        }
        uint8_t action_deserialized = buffer[1];
        return PlayerAction(static_cast<Action>(action_deserialized));
    }

};
#endif 
