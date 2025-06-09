#ifndef ACTION_H
#define ACTION_H

#include <cstdint>

enum class Action : uint8_t {
    MoveUp = 0x01,
    MoveDown = 0x02,
    MoveLeft = 0x03,
    MoveRight = 0x04,
    Shoot = 0x05,
    Reload = 0x06,
    EquipWeapon = 0x07,
    PlantBomb = 0x08,
    DefuseBomb = 0x09,
    Idle = 0x0A
};

#endif
