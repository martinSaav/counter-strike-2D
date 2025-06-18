//
// Created by matias on 17/06/25.
//

#ifndef GUNCONFIG_H
#define GUNCONFIG_H


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
    GunConfig(int max_ammo, int starting_reserve_ammo, int min_dmg, int max_dmg, int gun_price):
            max_ammo(max_ammo),
            starting_reserve_ammo(starting_reserve_ammo),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(gun_price),
            bullets_per_burst(0),
            shoot_cooldown(0),
            range(0),
            angle(0) {}

    GunConfig(int max_ammo, int starting_reserve_ammo, int min_dmg, int max_dmg, int gun_price,
              int bullets_per_burst, int shoot_cooldown):
            max_ammo(max_ammo),
            starting_reserve_ammo(starting_reserve_ammo),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(gun_price),
            bullets_per_burst(bullets_per_burst),
            shoot_cooldown(shoot_cooldown),
            range(0),
            angle(0) {}

    GunConfig(int max_ammo, int starting_reserve_ammo, int min_dmg, int max_dmg, int gun_price,
              int shoot_cooldown):
            max_ammo(max_ammo),
            starting_reserve_ammo(starting_reserve_ammo),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(gun_price),
            bullets_per_burst(0),
            shoot_cooldown(shoot_cooldown),
            range(0),
            angle(0) {}

    GunConfig(int min_dmg, int max_dmg, int range, double angle):
            max_ammo(0),
            starting_reserve_ammo(0),
            min_dmg(min_dmg),
            max_dmg(max_dmg),
            gun_price(0),
            bullets_per_burst(0),
            shoot_cooldown(0),
            range(range),
            angle(angle) {}
};


#endif  // GUNCONFIG_H
