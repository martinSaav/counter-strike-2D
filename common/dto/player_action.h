#ifndef PLAYER_ACTION_H
#define PLAYER_ACTION_H

#include <cstdint>

#include "../action.h"
#include "../message.h"

class PlayerAction: public MessageOf<MessageType::PlayerAction> {
private:
    Action action;
    uint16_t pos_mouse_x;
    uint16_t pos_mouse_y;

public:
    PlayerAction(Action action, uint16_t pos_mouse_x, uint16_t pos_mouse_y):
            action(action), pos_mouse_x(pos_mouse_x), pos_mouse_y(pos_mouse_y) {}

    void write_payload(ByteWriter& writer) const override {
        writer.enum_u8(action);
        writer.u16(pos_mouse_x);
        writer.u16(pos_mouse_y);
    }

    static PlayerAction read_payload(ByteReader& reader) {
        const auto action = reader.enum_u8<Action>();
        const uint16_t x = reader.u16();
        const uint16_t y = reader.u16();
        return PlayerAction(action, x, y);
    }

    Action get_action() const { return action; }

    uint16_t get_pos_mouse_x() const { return pos_mouse_x; }

    uint16_t get_pos_mouse_y() const { return pos_mouse_y; }
};
#endif
