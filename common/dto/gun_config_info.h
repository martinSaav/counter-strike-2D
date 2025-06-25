#ifndef GUN_CONFIG_INFO_H
#define GUN_CONFIG_INFO_H

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>

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

    void serialize(uint8_t* buffer) const {
        size_t offset = 0;

        int32_t max_ammo_net = htonl(max_ammo);
        memcpy(buffer + offset, &max_ammo_net, sizeof(max_ammo_net));
        offset += sizeof(max_ammo_net);

        int32_t starting_reserve_ammo_net = htonl(starting_reserve_ammo);
        memcpy(buffer + offset, &starting_reserve_ammo_net, sizeof(starting_reserve_ammo_net));
        offset += sizeof(starting_reserve_ammo_net);

        int32_t min_dmg_net = htonl(min_dmg);
        memcpy(buffer + offset, &min_dmg_net, sizeof(min_dmg_net));
        offset += sizeof(min_dmg_net);

        int32_t max_dmg_net = htonl(max_dmg);
        memcpy(buffer + offset, &max_dmg_net, sizeof(max_dmg_net));
        offset += sizeof(max_dmg_net);

        int32_t gun_price_net = htonl(gun_price);
        memcpy(buffer + offset, &gun_price_net, sizeof(gun_price_net));
        offset += sizeof(gun_price_net);

        int32_t bullets_per_burst_net = htonl(bullets_per_burst);
        memcpy(buffer + offset, &bullets_per_burst_net, sizeof(bullets_per_burst_net));
        offset += sizeof(bullets_per_burst_net);

        int32_t shoot_cooldown_net = htonl(shoot_cooldown);
        memcpy(buffer + offset, &shoot_cooldown_net, sizeof(shoot_cooldown_net));
        offset += sizeof(shoot_cooldown_net);

        int32_t range_net = htonl(range);
        memcpy(buffer + offset, &range_net, sizeof(range_net));
        offset += sizeof(range_net);

        memcpy(buffer + offset, &angle, sizeof(angle));
    }

    static GunConfigInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }

        size_t offset = 0;

        int32_t max_ammo;
        memcpy(&max_ammo, buffer + offset, sizeof(max_ammo));
        max_ammo = ntohl(max_ammo);
        offset += sizeof(max_ammo);

        int32_t starting_reserve_ammo;
        memcpy(&starting_reserve_ammo, buffer + offset, sizeof(starting_reserve_ammo));
        starting_reserve_ammo = ntohl(starting_reserve_ammo);
        offset += sizeof(starting_reserve_ammo);

        int32_t min_dmg;
        memcpy(&min_dmg, buffer + offset, sizeof(min_dmg));
        min_dmg = ntohl(min_dmg);
        offset += sizeof(min_dmg);

        int32_t max_dmg;
        memcpy(&max_dmg, buffer + offset, sizeof(max_dmg));
        max_dmg = ntohl(max_dmg);
        offset += sizeof(max_dmg);

        int32_t gun_price;
        memcpy(&gun_price, buffer + offset, sizeof(gun_price));
        gun_price = ntohl(gun_price);
        offset += sizeof(gun_price);

        int32_t bullets_per_burst;
        memcpy(&bullets_per_burst, buffer + offset, sizeof(bullets_per_burst));
        bullets_per_burst = ntohl(bullets_per_burst);
        offset += sizeof(bullets_per_burst);

        int32_t shoot_cooldown;
        memcpy(&shoot_cooldown, buffer + offset, sizeof(shoot_cooldown));
        shoot_cooldown = ntohl(shoot_cooldown);
        offset += sizeof(shoot_cooldown);

        int32_t range;
        memcpy(&range, buffer + offset, sizeof(range));
        range = ntohl(range);
        offset += sizeof(range);

        float angle;
        memcpy(&angle, buffer + offset, sizeof(angle));

        return GunConfigInfo(max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price,
                             bullets_per_burst, shoot_cooldown, range, angle);
    }

    size_t serialized_size() const {
        return sizeof(max_ammo) + sizeof(starting_reserve_ammo) + sizeof(min_dmg) +
               sizeof(max_dmg) + sizeof(gun_price) + sizeof(bullets_per_burst) +
               sizeof(shoot_cooldown) + sizeof(range) + sizeof(angle);
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
