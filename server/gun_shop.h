//
// Created by matias on 13/06/25.
//

#ifndef GUN_SHOP_H
#define GUN_SHOP_H
#include <memory>
#include <stdexcept>

#include "gun/gun.h"

struct CantBuyWeapon: public std::runtime_error {
    CantBuyWeapon(): std::runtime_error("Cant buy weapon") {}
};

class GunShop {
public:
    static std::unique_ptr<Gun> get_gun(Weapon weapon);
};


#endif  // GUN_SHOP_H
