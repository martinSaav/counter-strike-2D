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
    GunType type = GunType::Knife;
    std::pair<int, int> next_shoot;
    bool has_to_fire;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootResult fire_gun(Map& map, Player& owner, float current_time,
                         Position& current_position) override;
    void reset_shoots() override;

    [[nodiscard]] int calculate_damage(double distance) const;

public:
    explicit Knife(const GunConfig& knife_config):
            max_degree(knife_config.angle),
            knife_range(knife_config.range),
            min_dmg(knife_config.min_dmg),
            max_dmg(knife_config.max_dmg),
            has_to_fire(false) {}
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position) override;
    int get_gun_price() override { return 0; }
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::Knife, 0}; }
};


#endif  // KNIFE_H
