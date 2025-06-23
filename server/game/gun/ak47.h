//
// Created by matias on 03/06/25.
//

#ifndef AK47_H
#define AK47_H
#include <queue>
#include <utility>

#include "game_config.h"
#include "gun.h"


class GameConfig;

class Ak47: public Gun {
    const int max_ammo;
    const int bullets_per_burst;
    const int max_dmg;
    const int min_dmg;
    const int gun_price;
    const int miliseconds_per_shoot;
    const double base_precision;
    const double distance_precision_modifier;
    int current_ammo;
    int reserve_ammo;
    float time_since_last_shot;
    std::queue<std::pair<int, int>> shoots;
    GunType type = GunType::Primary;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                       Position& current_position) override;
    void reset_shoots() override;
    [[nodiscard]] bool calculate_if_bullet_hit(double distance) const;

public:
    explicit Ak47(const GunConfig& ak_config);
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position, float current_time) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::AK47, current_ammo}; }
    void add_magazine() override;
};


#endif  // AK47_H
