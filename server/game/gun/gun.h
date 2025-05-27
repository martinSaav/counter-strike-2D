//
// Created by matias on 26/05/25.
//

#ifndef GUN_H
#define GUN_H
#include <vector>

#include "bullet.h"
#include "gun_type.h"
#include "position.h"


class Gun {
    friend class GameStateUpdater;
    bool has_to_shoot();
    std::vector<Bullet> fire_gun();

public:
    virtual ~Gun() = default;

    virtual GunType get_gun_type();
    virtual void reload_gun() = 0;
    virtual void shoot_gun(Position& starting_position, Position final_position) = 0;
};


#endif  // GUN_H
