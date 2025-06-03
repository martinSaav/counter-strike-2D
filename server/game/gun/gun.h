//
// Created by matias on 26/05/25.
//

#ifndef GUN_H
#define GUN_H

#include <stdexcept>

#include "gun_type.h"
#include "position.h"


struct NoAmmo: public std::runtime_error {
    NoAmmo(): std::runtime_error("There is no ammunition") {}
};


class Gun {
    friend class GameStateUpdater;
    virtual bool has_to_shoot(float current_time);
    void fire_gun();

public:
    virtual ~Gun() = default;

    virtual GunType get_gun_type();
    virtual void reload_gun() = 0;
    virtual void shoot_gun(Position final_position) = 0;
};


#endif  // GUN_H
