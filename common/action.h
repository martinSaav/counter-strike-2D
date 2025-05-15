#ifndef ACTION_H
#define ACTION_H

#include <cstdint>

enum class Action : uint8_t {
    MoveUp = 0x01,
    MoveDown = 0x02,
    MoveLeft = 0x03,
    MoveRight = 0x04,
    Shoot = 0x05,
};

#endif
