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
    ShootResult fire_gun(Map& map, Player& owner, float current_time,
                         Position& current_position) override;
    void reset_shoots() override;

public:
    BombEncapsulator(): has_to_fire(false) {}
    bool has_finished_planting(float current_time) const;
    GunType get_gun_type() override { return type; }
    void reload_gun() override;
    void shoot_gun(Position final_position) override;
    int get_gun_price() override { return 0; }
    bool is_planting();
    WeaponInfo get_weapon_name() override { return WeaponInfo{Weapon::Bomb, 0}; }
};


#endif  // BOMBENCAPSULATOR_H
