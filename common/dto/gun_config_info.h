#ifndef GUN_CONFIG_INFO_H
#define GUN_CONFIG_INFO_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <netinet/in.h>

class GunConfigInfo {
private:
    uint16_t max_ammo;
    uint16_t starting_reserve_ammo;
    uint16_t min_dmg;
    uint16_t max_dmg;
    uint16_t gun_price;
    uint16_t bullets_per_burst;
    uint16_t shoot_cooldown;
    uint16_t range;
    float angle;

public:
    GunConfigInfo(uint16_t max_ammo, uint16_t starting_reserve_ammo,
              uint16_t min_dmg, uint16_t max_dmg, uint16_t gun_price,
              uint16_t bullets_per_burst, uint16_t shoot_cooldown,
              uint16_t range, float angle)
        : max_ammo(max_ammo), starting_reserve_ammo(starting_reserve_ammo),
          min_dmg(min_dmg), max_dmg(max_dmg), gun_price(gun_price),
          bullets_per_burst(bullets_per_burst), shoot_cooldown(shoot_cooldown),
          range(range), angle(angle) {}

    void serialize(uint8_t* buffer) const {
        uint16_t net_max_ammo = htons(max_ammo);
        uint16_t net_starting_reserve_ammo = htons(starting_reserve_ammo);
        uint16_t net_min_dmg = htons(min_dmg);
        uint16_t net_max_dmg = htons(max_dmg);
        uint16_t net_gun_price = htons(gun_price);
        uint16_t net_bullets_per_burst = htons(bullets_per_burst);
        uint16_t net_shoot_cooldown = htons(shoot_cooldown);
        uint16_t net_range = htons(range);

        std::memcpy(buffer, &net_max_ammo, sizeof(net_max_ammo));
        std::memcpy(buffer + 2, &net_starting_reserve_ammo, sizeof(net_starting_reserve_ammo));
        std::memcpy(buffer + 4, &net_min_dmg, sizeof(net_min_dmg));
        std::memcpy(buffer + 6, &net_max_dmg, sizeof(net_max_dmg));
        std::memcpy(buffer + 8, &net_gun_price, sizeof(net_gun_price));
        std::memcpy(buffer + 10, &net_bullets_per_burst, sizeof(net_bullets_per_burst));
        std::memcpy(buffer + 12, &net_shoot_cooldown, sizeof(net_shoot_cooldown));
        std::memcpy(buffer + 14, &net_range, sizeof(net_range));
        std::memcpy(buffer + 16, &angle, sizeof(angle));
    }

    static GunConfigInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }

        uint16_t max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price;
        uint16_t bullets_per_burst, shoot_cooldown, range;
        float angle;

        std::memcpy(&max_ammo, buffer, sizeof(max_ammo));
        std::memcpy(&starting_reserve_ammo, buffer + 2, sizeof(starting_reserve_ammo));
        std::memcpy(&min_dmg, buffer + 4, sizeof(min_dmg));
        std::memcpy(&max_dmg, buffer + 6, sizeof(max_dmg));
        std::memcpy(&gun_price, buffer + 8, sizeof(gun_price));
        std::memcpy(&bullets_per_burst, buffer + 10, sizeof(bullets_per_burst));
        std::memcpy(&shoot_cooldown, buffer + 12, sizeof(shoot_cooldown));
        std::memcpy(&range, buffer + 14, sizeof(range));
        std::memcpy(&angle, buffer + 16, sizeof(angle));

        return GunConfigInfo(ntohs(max_ammo), ntohs(starting_reserve_ammo),
                         ntohs(min_dmg), ntohs(max_dmg), ntohs(gun_price),
                         ntohs(bullets_per_burst), ntohs(shoot_cooldown),
                         ntohs(range), angle);
    }

    size_t serialized_size() const {
        return 16 + sizeof(angle);
    }

    uint16_t get_max_ammo() const { return max_ammo; }

    uint16_t get_starting_reserve_ammo() const { return starting_reserve_ammo; }

    uint16_t get_min_dmg() const { return min_dmg; }

    uint16_t get_max_dmg() const { return max_dmg; }

    uint16_t get_gun_price() const { return gun_price; }

    uint16_t get_bullets_per_burst() const { return bullets_per_burst; }

    uint16_t get_shoot_cooldown() const { return shoot_cooldown; }

    uint16_t get_range() const { return range; }

    float get_angle() const { return angle; }
};

#endif
