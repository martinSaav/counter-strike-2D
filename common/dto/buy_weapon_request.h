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


enum class Weapon : uint8_t {
    Glock = 0x01,
    AK47 = 0x02,
    M4 = 0x03,
    AWP = 0x04,
};

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

    size_t serialized_size() const override { return 4; }

    Weapon get_weapon() const { return weapon; }

    static BuyWeaponRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 4) {
            throw std::runtime_error("");
        }
        uint8_t weapon_deserialized = buffer[3];
        return BuyWeaponRequest(static_cast<Weapon>(weapon_deserialized));
    }

    MessageType type() const override { return this->message_type; }
};
#endif
