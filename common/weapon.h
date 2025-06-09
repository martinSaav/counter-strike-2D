#ifndef WEAPON_H
#define WEAPON_H

#include <cstdint>

enum class Weapon : uint8_t {
    Knife = 0x00,
    Glock = 0x01,
    AK47 = 0x02,
    M3 = 0x03,
    AWP = 0x04,
};


#endif
