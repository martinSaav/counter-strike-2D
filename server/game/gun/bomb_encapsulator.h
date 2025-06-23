//
// Created by matias on 09/06/25.
//

#ifndef BOMBENCAPSULATOR_H
#define BOMBENCAPSULATOR_H
#include "bomb.h"
#include "gun.h"

class Player;

class BombEncapsulator: public Gun {
    friend class Player;
    GunType type = GunType::Bomb;
    Bomb bomb;
    bool has_to_fire;

    [[nodiscard]] bool has_to_shoot(float current_time) override;
    ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                       Position& current_position) override;
    void reset_shoots() override;
    void reset_time() override {}

public:
    explicit BombEncapsulator(const GameConfig& config): bomb(config), has_to_fire(false) {}
    [[nodiscard]] bool has_finished_planting(float current_time) const;
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position, float time) override;
    int get_gun_price() override { return 0; }
    [[nodiscard]] bool is_planting() const;
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::Bomb, 0}; }
    void add_magazine() override {}
};


#endif  // BOMBENCAPSULATOR_H
