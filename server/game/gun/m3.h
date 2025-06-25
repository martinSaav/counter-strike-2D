//
// Created by matias on 21/06/25.
//

#ifndef M3_H
#define M3_H
#include <utility>

#include "gun.h"
#include "gun_config.h"


class M3: public Gun {
    const int max_ammo;
    const int gun_price;
    const int min_dmg;
    const int max_dmg;
    const double degree;
    const int range;
    const int shoot_cooldown;
    const double base_precision;
    const double distance_precision_modifier;
    float time_since_last_shot;
    int current_ammo;
    int reserve_ammo;
    GunType type = GunType::Primary;
    std::pair<int, int> next_shoot;
    bool has_to_fire;
    bool has_to_shoot(float current_time) override;
    ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                       Position& current_position) override;
    void reset_shoots() override;
    [[nodiscard]] int calculate_damage(double distance) const;
    [[nodiscard]] bool calculate_if_bullet_hit(double distance) const;
    void reset_time() override;

public:
    explicit M3(const GunConfig& m3_config);
    GunType get_gun_type() override;
    bool can_reload() override;
    void reload_gun() override;
    void shoot_gun(Position final_position, float current_time) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override;
    void add_magazine() override;
};


#endif  // M3_H
