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
#include "../weapon.h"


class PlayerAction: public Message {
private:
    MessageType message_type = MessageType::PlayerAction;
    Action action;
    uint16_t pos_mouse_x;
    uint16_t pos_mouse_y;
    Weapon weapon;

public:
    explicit PlayerAction(Action action, uint16_t pos_mouse_x, uint16_t pos_mouse_y, Weapon weapon):
            action(action), pos_mouse_x(pos_mouse_x), pos_mouse_y(pos_mouse_y), weapon(weapon) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t payload_length = htons(6);
        std::memcpy(buffer + 1, &payload_length, sizeof(payload_length));
        buffer[3] = static_cast<uint8_t>(action);
        uint16_t pos_mouse_x_be = htons(pos_mouse_x);
        uint16_t pos_mouse_y_be = htons(pos_mouse_y);
        std::memcpy(buffer + 4, &pos_mouse_x_be, sizeof(pos_mouse_x_be));
        std::memcpy(buffer + 6, &pos_mouse_y_be, sizeof(pos_mouse_y_be));
        buffer[8] = static_cast<uint8_t>(weapon);
    }

    size_t serialized_size() const override { return 9; }

    Action get_action() const { return action; }
    uint16_t get_pos_mouse_x() const { return pos_mouse_x; }
    uint16_t get_pos_mouse_y() const { return pos_mouse_y; }
    Weapon get_weapon() const { return weapon; }

    static PlayerAction deserialize(const uint8_t* buffer, size_t size) {
        if (size < 4) {
            throw std::runtime_error("");
        }
        uint8_t action_deserialized = buffer[3];
        uint16_t pos_mouse_x, pos_mouse_y;
        std::memcpy(&pos_mouse_x, buffer + 4, sizeof(pos_mouse_x));
        std::memcpy(&pos_mouse_y, buffer + 6, sizeof(pos_mouse_y));
        pos_mouse_x = ntohs(pos_mouse_x);
        pos_mouse_y = ntohs(pos_mouse_y);
        uint8_t weapon_deserialized = buffer[8];
        return PlayerAction(static_cast<Action>(action_deserialized), pos_mouse_x, pos_mouse_y,
                            static_cast<Weapon>(weapon_deserialized));
    }

    MessageType type() const override { return this->message_type; }
};
#endif
