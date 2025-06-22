//
// Created by matias on 13/06/25.
//

#include "gun_shop.h"

#include "gun/ak47.h"
#include "gun/awp.h"
#include "gun/glock.h"
#include "gun/m3.h"

std::unique_ptr<Gun> GunShop::get_gun(const Weapon weapon, const GameConfig& config) {
    switch (weapon) {
        case Weapon::AK47: {
            return std::make_unique<Ak47>(config.ak_config);
        }
        case Weapon::Glock: {
            return std::make_unique<Glock>(config.glock_config);
        }
        case Weapon::AWP: {
            return std::make_unique<Awp>(config.awp_config);
        }
        case Weapon::M3: {
            return std::make_unique<M3>(config.m3_config);
        }
        default: {
            throw CantBuyWeapon();
        }
    }
}
