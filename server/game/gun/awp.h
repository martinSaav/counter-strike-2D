//
// Created by matias on 18/06/25.
//

#ifndef AWP_H
#define AWP_H
#include <queue>
#include <utility>

#include "gun.h"
#include "gun_config.h"


class Awp: public Gun {
    const int max_ammo;
    const int max_dmg;
    const int min_dmg;
    const int gun_price;
    const int shoot_cooldown;
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
    void reset_time() override;

public:
    explicit Awp(const GunConfig& awp_config);
    GunType get_gun_type() override { return type; }
    bool can_reload() override;
    void reload_gun() override;
    void shoot_gun(Position final_position, float current_time) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::AWP, current_ammo}; }
    void add_magazine() override;
};


#endif  // AWP_H
