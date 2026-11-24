//
// Created by matias on 03/06/25.
//

#ifndef AK47_H
#define AK47_H
#include <utility>

#include "game_config.h"
#include "gun.h"


class GameConfig;

// Dispara rafagas de bullets_per_burst balas separadas por shoot_cooldown. Si se mantiene el
// click, la rafaga siguiente arranca burst_cooldown despues de la ultima bala. Cada bala sale hacia
// donde apunta el jugador en ese momento, no hacia donde apuntaba al empezar la rafaga.
class Ak47: public Gun {
    const int max_ammo;
    const int bullets_per_burst;
    const int max_dmg;
    const int min_dmg;
    const int gun_price;
    const int miliseconds_per_shoot;
    const int miliseconds_between_bursts;
    const double base_precision;
    const double distance_precision_modifier;
    int current_ammo;
    int reserve_ammo;
    float time_since_last_shot;
    float time_since_burst_ended;
    int bullets_left_in_burst;
    std::pair<int, int> target;
    GunType type = GunType::Primary;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                       Position& current_position) override;
    void reset_shoots() override;
    [[nodiscard]] bool calculate_if_bullet_hit(double distance) const;
    void reset_time() override;

public:
    explicit Ak47(const GunConfig& ak_config);
    GunType get_gun_type() override { return type; }
    bool can_reload() override;
    void reload_gun() override;
    void shoot_gun(Position final_position, float current_time) override;
    int get_gun_price() override;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::AK47, current_ammo}; }
    void add_magazine() override;
};


#endif  // AK47_H
