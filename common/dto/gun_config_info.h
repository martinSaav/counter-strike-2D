#ifndef GUN_CONFIG_INFO_H
#define GUN_CONFIG_INFO_H

#include <cstdint>

#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"

class GunConfigInfo {
private:
    int32_t max_ammo;
    int32_t starting_reserve_ammo;
    int32_t min_dmg;
    int32_t max_dmg;
    int32_t gun_price;
    int32_t bullets_per_burst;
    int32_t shoot_cooldown;
    int32_t range;
    float angle;

public:
    GunConfigInfo(int32_t max_ammo, int32_t starting_reserve_ammo, int32_t min_dmg, int32_t max_dmg,
                  int32_t gun_price, int32_t bullets_per_burst, int32_t shoot_cooldown,
                  int32_t range, float angle):
            max_ammo(max_ammo),
            starting_reserve_ammo(starting_reserve_ammo),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(gun_price),
            bullets_per_burst(bullets_per_burst),
            shoot_cooldown(shoot_cooldown),
            range(range),
            angle(angle) {}

    void write(ByteWriter& writer) const {
        writer.i32(max_ammo);
        writer.i32(starting_reserve_ammo);
        writer.i32(min_dmg);
        writer.i32(max_dmg);
        writer.i32(gun_price);
        writer.i32(bullets_per_burst);
        writer.i32(shoot_cooldown);
        writer.i32(range);
        writer.f32(angle);
    }

    static GunConfigInfo read(ByteReader& reader) {
        const int32_t max_ammo = reader.i32();
        const int32_t starting_reserve_ammo = reader.i32();
        const int32_t min_dmg = reader.i32();
        const int32_t max_dmg = reader.i32();
        const int32_t gun_price = reader.i32();
        const int32_t bullets_per_burst = reader.i32();
        const int32_t shoot_cooldown = reader.i32();
        const int32_t range = reader.i32();
        const float angle = reader.f32();
        return GunConfigInfo(max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price,
                             bullets_per_burst, shoot_cooldown, range, angle);
    }

    int32_t get_max_ammo() const { return max_ammo; }

    int32_t get_starting_reserve_ammo() const { return starting_reserve_ammo; }

    int32_t get_min_dmg() const { return min_dmg; }

    int32_t get_max_dmg() const { return max_dmg; }

    int32_t get_gun_price() const { return gun_price; }

    int32_t get_bullets_per_burst() const { return bullets_per_burst; }

    int32_t get_shoot_cooldown() const { return shoot_cooldown; }

    int32_t get_range() const { return range; }

    float get_angle() const { return angle; }
};

#endif
