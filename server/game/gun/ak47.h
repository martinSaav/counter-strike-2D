//
// Created by matias on 03/06/25.
//

#ifndef AK47_H
#define AK47_H
#include <queue>
#include <utility>

#include "gun.h"


class Ak47: public Gun {
    int current_ammo;
    int reserve_ammo;
    float time_since_last_shot;
    std::queue<std::pair<int, int>> shoots;
    GunType type = GunType::Primary;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootResult fire_gun(Map& map, Player& owner, float current_time,
                         Position& current_position) override;
    void reset_shoots() override;

public:
    Ak47();
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::AK47, current_ammo}; }
};


#endif  // AK47_H
