//
// Created by matias on 03/06/25.
//

#include "ak47.h"

#include <algorithm>

bool Ak47::has_to_shoot(const float current_time) {
    return !shoots.empty() && (current_time - time_since_last_shot > miliseconds_per_shoot);
}

void Ak47::reload_gun() {
    const int max_ammo_to_reload = max_ammo - current_ammo;
    if (const int ammo_to_reload = std::min(reserve_ammo, max_ammo_to_reload); ammo_to_reload > 0) {
        reserve_ammo -= ammo_to_reload;
        current_ammo += ammo_to_reload;
    }
}

void Ak47::shoot_gun(const Position final_position) {
    if (current_ammo == 0) {
        throw NoAmmo();
    }
    const auto pos = final_position.get_position();
    if (shoots.empty()) {
        const int bullets_to_shoot = std::min(bullets_per_burst, current_ammo);
        current_ammo -= bullets_to_shoot;
        for (int i = 0; i < bullets_to_shoot; i++) {
            shoots.push(pos);
        }
    } else {
        if (shoots.size() == 1) {
            current_ammo--;
            shoots.push(pos);
        }
    }
}
