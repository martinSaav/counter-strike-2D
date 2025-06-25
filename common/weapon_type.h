#ifndef WEAPON_TYPE_H
#define WEAPON_TYPE_H

#include <cstdint>

enum class WeaponType : uint8_t {
    Primary = 0x01,
    Secondary = 0x02,
    Knife = 0x03,
    Bomb = 0x04,
};

#endif
