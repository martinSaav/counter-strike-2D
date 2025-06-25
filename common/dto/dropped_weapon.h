#ifndef DROPPED_WEAPON_H
#define DROPPED_WEAPON_H

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <netinet/in.h>

#include "../weapon.h"

class DroppedWeapon {
private:
    Weapon weapon;
    uint16_t pos_x;
    uint16_t pos_y;

public:
    DroppedWeapon(Weapon weapon, uint16_t pos_x, uint16_t pos_y):
            weapon(weapon), pos_x(pos_x), pos_y(pos_y) {}

    void serialize(uint8_t* buffer) const {
        buffer[0] = static_cast<uint8_t>(weapon);
        uint16_t pos_x_be = htons(pos_x);
        uint16_t pos_y_be = htons(pos_y);
        std::memcpy(buffer + 1, &pos_x_be, sizeof(pos_x_be));
        std::memcpy(buffer + 3, &pos_y_be, sizeof(pos_y_be));
    }

    static DroppedWeapon deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }

        Weapon weapon = static_cast<Weapon>(buffer[0]);
        uint16_t x, y;
        std::memcpy(&x, buffer + 1, sizeof(x));
        std::memcpy(&y, buffer + 3, sizeof(y));
        x = ntohs(x);
        y = ntohs(y);
        return DroppedWeapon(weapon, x, y);
    }

    size_t serialized_size() const { return sizeof(weapon) + 2 * sizeof(uint16_t); }

    Weapon get_weapon() const { return weapon; }

    uint16_t get_pos_x() const { return pos_x; }

    uint16_t get_pos_y() const { return pos_y; }
};

#endif
