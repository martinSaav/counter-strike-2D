//
// Created by matias on 17/06/25.
//

#ifndef GUNCONFIG_H
#define GUNCONFIG_H
#include "dto/gun_config_info.h"


class GunConfig {
public:
    const int max_ammo;
    const int starting_reserve_ammo;
    const int min_dmg;
    const int max_dmg;
    const int gun_price;
    const int bullets_per_burst;
    const int shoot_cooldown;
    const int range;
    const double angle;

    GunConfig(int max_ammo, int starting_reserve_ammo, int min_dmg, int max_dmg, int gun_price,
              int range, double angle, int shoot_cooldown, int bullets_per_burst):
            max_ammo(max_ammo),
            starting_reserve_ammo(starting_reserve_ammo),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(gun_price),
            bullets_per_burst(bullets_per_burst),
            shoot_cooldown(shoot_cooldown),
            range(range),
            angle(angle) {}
    [[nodiscard]] GunConfigInfo get_info() const {
        return GunConfigInfo{max_ammo,
                             starting_reserve_ammo,
                             min_dmg,
                             max_dmg,
                             gun_price,
                             bullets_per_burst,
                             shoot_cooldown,
                             range,
                             static_cast<float>(angle)};
    }
};


#endif  // GUNCONFIG_H
