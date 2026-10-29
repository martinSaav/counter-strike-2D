#ifndef BUY_WEAPON_REQUEST_H
#define BUY_WEAPON_REQUEST_H

#include "../message.h"
#include "../weapon.h"

class BuyWeaponRequest: public MessageOf<MessageType::BuyWeaponRequest> {
private:
    Weapon weapon;

public:
    explicit BuyWeaponRequest(Weapon weapon): weapon(weapon) {}

    void write_payload(ByteWriter& writer) const override { writer.enum_u8(weapon); }

    static BuyWeaponRequest read_payload(ByteReader& reader) {
        return BuyWeaponRequest(reader.enum_u8<Weapon>());
    }

    Weapon get_weapon() const { return weapon; }
};
#endif
