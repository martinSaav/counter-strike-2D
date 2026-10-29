#ifndef DROPPED_WEAPON_H
#define DROPPED_WEAPON_H

#include <cstdint>

#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"
#include "../weapon.h"

class DroppedWeapon {
private:
    Weapon weapon;
    uint16_t pos_x;
    uint16_t pos_y;

public:
    DroppedWeapon(Weapon weapon, uint16_t pos_x, uint16_t pos_y):
            weapon(weapon), pos_x(pos_x), pos_y(pos_y) {}

    void write(ByteWriter& writer) const {
        writer.enum_u8(weapon);
        writer.u16(pos_x);
        writer.u16(pos_y);
    }

    static DroppedWeapon read(ByteReader& reader) {
        const auto weapon = reader.enum_u8<Weapon>();
        const uint16_t x = reader.u16();
        const uint16_t y = reader.u16();
        return DroppedWeapon(weapon, x, y);
    }

    Weapon get_weapon() const { return weapon; }

    uint16_t get_pos_x() const { return pos_x; }

    uint16_t get_pos_y() const { return pos_y; }
};

#endif
