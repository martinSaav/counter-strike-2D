//
// Created by matias on 26/05/25.
//

#ifndef GUN_H
#define GUN_H

#include <stdexcept>

#include "gun_type.h"
#include "position.h"
#include "shoot_result.h"
#include "weapon_info.h"

#define gun_hitbox_width 30
#define gun_hitbox_height 30

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
    virtual ShootResult fire_gun(Map& map, Player& owner, float current_time,
                                 Position& current_position) = 0;
    virtual void reset_shoots() = 0;

public:
    virtual ~Gun() = default;

    virtual GunType get_gun_type() = 0;
    virtual void reload_gun() = 0;
    virtual void shoot_gun(Position final_position) = 0;
    virtual int get_gun_price() = 0;
    virtual WeaponInfo get_weapon_name() = 0;
};


#endif  // GUN_H
