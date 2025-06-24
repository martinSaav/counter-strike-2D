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
    SetKnife = 0x08,
    SetPrimaryWeapon = 0x09,
    SetSecondaryWeapon = 0x0A,
    SetBomb = 0x0B,
    PlantBomb = 0x0C,
    DefuseBomb = 0x0D,
    Idle = 0x0E,
    BuyingWeapon = 0x0F,
    BuyingAmmo = 0x10,
};

#endif
