//
// Created by matias on 03/06/25.
//

#ifndef GLOCK_H
#define GLOCK_H

#include <utility>

#include "gun.h"
#include "gun_config.h"

class Glock: public Gun {
    const int max_ammo;
    const int gun_price;
    const int min_dmg;
    const int max_dmg;
    const int shoot_cooldown;
    const double base_precision;
    const double distance_precision_modifier;
    int current_ammo;
    int reserve_ammo;
    float time_since_last_shot;
    GunType type = GunType::Secondary;
    std::pair<int, int> next_shoot;
    bool has_to_fire;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                       Position& current_position) override;
    void reset_shoots() override;
    [[nodiscard]] bool calculate_if_bullet_hit(double distance) const;
    void reset_time() override;

public:
    explicit Glock(const GunConfig& glock_config);
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position, float current_time) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::Glock, current_ammo}; }
    void add_magazine() override;
};


#endif  // GLOCK_H
