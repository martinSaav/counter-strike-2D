//
// Created by matias on 26/05/25.
//

#ifndef GUN_H
#define GUN_H

#include <stdexcept>

#include "gun_type.h"
#include "position.h"
#include "shoot_info.h"
#include "weapon_info.h"

#define gun_hitbox_width 3
#define gun_hitbox_height 3

struct NoAmmo: public std::runtime_error {
    NoAmmo(): std::runtime_error("There is no ammunition") {}
};

struct DontHaveToShoot: public std::runtime_error {
    DontHaveToShoot(): std::runtime_error("Gun doesnt have to shoot") {}
};

class Player;
class Map;

class Gun {
    friend class Player;
    virtual bool has_to_shoot(float current_time) = 0;
    virtual ShootInfo fire_gun(Map& map, Player& owner, float current_time,
                               Position& current_position) = 0;
    virtual void reset_shoots() = 0;
    virtual void reset_time() = 0;

protected:
    static double generate_random_number(double a, double b);

public:
    virtual ~Gun() = default;

    virtual GunType get_gun_type() = 0;
    virtual bool can_reload() = 0;
    virtual void reload_gun() = 0;
    virtual void shoot_gun(Position final_position, float current_time) = 0;
    virtual int get_gun_price() = 0;
    virtual WeaponInfo get_weapon_name() = 0;
    virtual void add_magazine() = 0;
};


#endif  // GUN_H
