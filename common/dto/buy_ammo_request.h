#ifndef BUY_AMMO_REQUEST_H
#define BUY_AMMO_REQUEST_H

#include "../message.h"
#include "../weapon_type.h"

class BuyAmmoRequest: public MessageOf<MessageType::BuyAmmoRequest> {
private:
    WeaponType weapon_type;

public:
    explicit BuyAmmoRequest(WeaponType weapon_type): weapon_type(weapon_type) {}

    void write_payload(ByteWriter& writer) const override { writer.enum_u8(weapon_type); }

    static BuyAmmoRequest read_payload(ByteReader& reader) {
        return BuyAmmoRequest(reader.enum_u8<WeaponType>());
    }

    WeaponType get_weapon_type() const { return weapon_type; }
};

#endif
