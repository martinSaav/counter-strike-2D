//
// Created by matias on 13/06/25.
//

#include "gun_shop.h"

#include "gun/ak47.h"
#include "gun/glock.h"

std::unique_ptr<Gun> GunShop::get_gun(Weapon weapon) {
    switch (weapon) {
        case Weapon::AK47: {
            return std::make_unique<Ak47>();
        }
        case Weapon::Glock: {
            return std::make_unique<Glock>();
        }
        case Weapon::Knife: {
            throw CantBuyWeapon();
        }
        case Weapon::Bomb: {
            throw CantBuyWeapon();
        }
        default: {
            throw CantBuyWeapon();
        }
    }
}
