#ifndef KILL_INFO_H
#define KILL_INFO_H

#include <string>
#include <utility>

#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"
#include "../weapon.h"

// Una muerte, para el registro de bajas del cliente. Si la mato la bomba, killer queda vacio y
// weapon es Weapon::Bomb.
class KillInfo {
private:
    std::string killer;
    std::string victim;
    Weapon weapon;

public:
    KillInfo(std::string killer, std::string victim, Weapon weapon):
            killer(std::move(killer)), victim(std::move(victim)), weapon(weapon) {}

    void write(ByteWriter& writer) const {
        writer.string(killer);
        writer.string(victim);
        writer.enum_u8(weapon);
    }

    static KillInfo read(ByteReader& reader) {
        std::string killer = reader.string();
        std::string victim = reader.string();
        const auto weapon = reader.enum_u8<Weapon>();
        return KillInfo(std::move(killer), std::move(victim), weapon);
    }

    const std::string& get_killer() const { return killer; }

    const std::string& get_victim() const { return victim; }

    Weapon get_weapon() const { return weapon; }
};

#endif
