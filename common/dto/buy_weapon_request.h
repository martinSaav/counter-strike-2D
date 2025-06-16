#ifndef BUY_WEAPON_REQUEST_H
#define BUY_WEAPON_REQUEST_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"
#include "../weapon.h"


class BuyWeaponRequest: public Message {
private:
    MessageType message_type = MessageType::BuyWeaponRequest;
    Weapon weapon;

public:
    explicit BuyWeaponRequest(Weapon weapon): weapon(weapon) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(1);
        memcpy(buffer + 1, &length, sizeof(length));
        buffer[3] = static_cast<uint8_t>(weapon);
    }

    static BuyWeaponRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 4) {
            throw std::runtime_error("");
        }
        uint8_t weapon_deserialized = buffer[3];
        return BuyWeaponRequest(static_cast<Weapon>(weapon_deserialized));
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE;  // header
        size += sizeof(weapon);     // weapon
        return size;
    }

    Weapon get_weapon() const { return weapon; }
};
#endif
