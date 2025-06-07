//
// Created by matias on 03/06/25.
//

#include "glock.h"

#include <algorithm>
#include <cmath>

int Glock::get_gun_price() { return glock_price; }

void Glock::shoot_gun(Position final_position) {
    if (current_ammo == 0) {
        throw NoAmmo();
    }
    const auto pos = final_position.get_position();
    next_shoot = pos;
    has_to_fire = true;
}

void Glock::reset_shoots() { has_to_fire = false; }


bool Glock::has_to_shoot(float current_time) { return has_to_fire; }


ShootResult Glock::fire_gun(Map& map, Player& owner, float current_time,
                            Position& current_position) {
    if (!has_to_shoot(current_time)) {
        return ShootResult{};
    }
    auto [x, y] = current_position.get_position();
    auto [final_x, final_y] = next_shoot;
    current_ammo--;
    auto player_hit_o = map.trace_bullet_path(x, y, Position(final_x, final_y), owner);
    if (player_hit_o.has_value()) {
        const auto& player_hit = player_hit_o.value();
        auto [player_hit_x, player_hit_y] = player_hit->get_location();
        const double distance = std::sqrt(pow(x - player_hit_x, 2) + pow(y - player_hit_y, 2));
        const int damage_before_distance = min_dmg + std::rand() % (max_dmg - min_dmg + 1);
        const double damage = static_cast<double>(damage_before_distance) / 1 + (distance / 10);
        return ShootResult{static_cast<int>(damage), player_hit};
    }
    return ShootResult{};
}

void Glock::reload_gun() {
    if (current_ammo == 0 && reserve_ammo == 0) {
        throw NoAmmo();
    }
    const int max_ammo_to_reload = max_ammo - current_ammo;
    if (const int ammo_to_reload = std::min(reserve_ammo, max_ammo_to_reload); ammo_to_reload > 0) {
        reserve_ammo -= ammo_to_reload;
        current_ammo += ammo_to_reload;
    }
}
