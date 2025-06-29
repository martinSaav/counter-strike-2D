//
// Created by matias on 06/06/25.
//

#ifndef KNIFE_H
#define KNIFE_H
#include <utility>

#include "gun.h"
#include "gun_config.h"

class Knife: public Gun {
    const double max_degree;
    const int knife_range;
    const int min_dmg;
    const int max_dmg;
    const int shoot_cooldown;
    float time_since_last_shot;
    GunType type = GunType::Knife;
    std::pair<int, int> next_shoot;
    bool has_to_fire;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                       Position& current_position) override;
    void reset_shoots() override;
    void reset_time() override;

    [[nodiscard]] int calculate_damage(double distance) const;

public:
    explicit Knife(const GunConfig& knife_config):
            max_degree(knife_config.angle),
            knife_range(knife_config.range),
            min_dmg(knife_config.min_dmg),
            max_dmg(knife_config.max_dmg),
            shoot_cooldown(knife_config.shoot_cooldown),
            time_since_last_shot(0),
            has_to_fire(false) {}
    GunType get_gun_type() override { return type; }
    bool can_reload() override { return false; }
    void reload_gun() override;
    void shoot_gun(Position final_position, float current_time) override;
    int get_gun_price() override { return 0; }
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::Knife, 0}; }
    void add_magazine() override {}
};


#endif  // KNIFE_H
