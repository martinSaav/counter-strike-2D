//
// Created by matias on 03/06/25.
//

#ifndef AK47_H
#define AK47_H
#include <queue>
#include <utility>

#include "gun.h"

#define miliseconds_per_shoot 2500
#define max_ammo 30
#define max_reserve_ammo 60
#define bullets_per_burst 3
#define ak_price 600
#define max_dmg 40
#define min_dmg 20
#define max_range 300

class Ak47: public Gun {
    int current_ammo;
    int reserve_ammo;
    float time_since_last_shot;
    std::queue<std::pair<int, int>> shoots;
    GunType type = GunType::Primary;

    Ak47(): current_ammo(max_ammo), reserve_ammo(max_reserve_ammo), time_since_last_shot(0) {}

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


#endif  // AK47_H
