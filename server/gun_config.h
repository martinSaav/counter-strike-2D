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
    const double base_precision;
    const double distance_precision_modifier;
    // Solo la AK: pausa (ms) entre el fin de una rafaga y el comienzo de la siguiente. Su
    // shoot_cooldown es el tiempo entre las balas de una misma rafaga.
    const int burst_cooldown;

    GunConfig(int max_ammo, int starting_reserve_ammo, int min_dmg, int max_dmg, int gun_price,
              int range, double angle, int shoot_cooldown, int bullets_per_burst,
              double base_precision, double distance_precision_modifier, int burst_cooldown = 0):
            max_ammo(max_ammo),
            starting_reserve_ammo(starting_reserve_ammo),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(gun_price),
            bullets_per_burst(bullets_per_burst),
            shoot_cooldown(shoot_cooldown),
            range(range),
            angle(angle),
            base_precision(base_precision),
            distance_precision_modifier(distance_precision_modifier),
            burst_cooldown(burst_cooldown) {}
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
