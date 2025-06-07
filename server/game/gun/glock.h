//
// Created by matias on 03/06/25.
//

#ifndef GLOCK_H
#define GLOCK_H

#include <utility>

#include "gun.h"
#define max_ammo 20
#define max_reserve_ammo 40
#define glock_price 200
#define max_dmg 40
#define min_dmg 20
#define max_range 150

class Glock: public Gun {
    int current_ammo;
    int reserve_ammo;
    GunType type = GunType::Secondary;
    std::pair<int, int> next_shoot;
    bool has_to_fire;

    Glock(): current_ammo(max_ammo), reserve_ammo(max_reserve_ammo), has_to_fire(false) {}

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootResult fire_gun(Map& map, Player& owner, float current_time,
                         Position& current_position) override;
    void reset_shoots() override;

public:
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position) override;
    int get_gun_price() override;
};


#endif  // GLOCK_H
