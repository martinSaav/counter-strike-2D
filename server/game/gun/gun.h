//
// Created by matias on 26/05/25.
//

#ifndef GUN_H
#define GUN_H
#include <vector>

#include "server/game/position.h"

#include "bullet.h"
#include "gun_type.h"


class Gun {
    friend class GameStateUpdater;
    bool has_to_shoot();
    void fire_gun();

public:
    virtual ~Gun() = default;

    virtual GunType get_gun_type();
    virtual void reload_gun() = 0;
    virtual void shoot_gun(Position& starting_position, Position final_position) = 0;
};


#endif  // GUN_H
