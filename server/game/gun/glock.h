//
// Created by matias on 03/06/25.
//

#ifndef GLOCK_H
#define GLOCK_H
#include "gun.h"


class Glock: public Gun {
    GunType type = GunType::Secondary;
};


#endif  // GLOCK_H
