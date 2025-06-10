//
// Created by matias on 03/06/25.
//

#ifndef GLOCK_H
#define GLOCK_H

#include <utility>

#include "gun.h"

class Glock: public Gun {
    int current_ammo;
    int reserve_ammo;
    GunType type = GunType::Secondary;
    std::pair<int, int> next_shoot;
    bool has_to_fire;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootResult fire_gun(Map& map, Player& owner, float current_time,
                         Position& current_position) override;
    void reset_shoots() override;

public:
    Glock();
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::Glock, current_ammo}; }
};


#endif  // GLOCK_H
