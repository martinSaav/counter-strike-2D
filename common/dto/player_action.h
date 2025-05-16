#ifndef PLAYER_ACTION_H
#define PLAYER_ACTION_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../action.h"
#include "../message.h"
#include "../message_type.h"


class PlayerAction: public Message {
private:
    MessageType message_type = MessageType::PlayerAction;
    Action action;

public:
    explicit PlayerAction(Action action): action(action) {}
    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        buffer[1] = static_cast<uint8_t>(action);
    }

    size_t serialized_size() const override { return 2; }

    uint8_t get_action() const { return static_cast<uint8_t>(action); }

    static PlayerAction deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint8_t action_deserialized = buffer[1];
        return PlayerAction(static_cast<Action>(action_deserialized));
    }
};
#endif
