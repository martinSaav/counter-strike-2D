#ifndef BUY_AMMO_REQUEST_H
#define BUY_AMMO_REQUEST_H


#include <cstring>
#include <stdexcept>
#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"
#include "../weapon_type.h"



class BuyAmmoRequest: public Message {
private:    
    MessageType message_type = MessageType::BuyAmmoRequest;
    WeaponType weapon_type;

public:
    explicit BuyAmmoRequest(WeaponType weapon_type): weapon_type(weapon_type) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(1);
        memcpy(buffer + 1, &length, sizeof(length));
        buffer[3] = static_cast<uint8_t>(weapon_type);
    }

    static BuyAmmoRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 4) {
            throw std::runtime_error("");
        }
        uint8_t weapon_type_deserialized = buffer[3];
        return BuyAmmoRequest(static_cast<WeaponType>(weapon_type_deserialized));
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE;  // header
        size += sizeof(weapon_type);     // weapon_type
        return size;
    }

    WeaponType get_weapon_type() const { return weapon_type; }
    
};

#endif
