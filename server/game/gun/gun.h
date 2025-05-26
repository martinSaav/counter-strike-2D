//
// Created by matias on 26/05/25.
//

#ifndef GUN_H
#define GUN_H
#include "bullet.h"
#include "gun_type.h"
#include "position.h"


class Gun {
public:
    virtual ~Gun() = default;

    virtual GunType get_gun_type();
    virtual void reload_gun() = 0;
    virtual Bullet shoot_gun(Position position) = 0;
};


#endif  // GUN_H
