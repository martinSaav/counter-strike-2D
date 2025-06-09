#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>

enum class Weapon : uint8_t {
    None = 0x00,
    Knife = 0x01,
    Bomb = 0x02,
    Glock = 0x03,
    AK47 = 0x04,
    M3 = 0x05,
    AWP = 0x06,
};


#endif
