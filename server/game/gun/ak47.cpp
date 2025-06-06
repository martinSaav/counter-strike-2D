//
// Created by matias on 03/06/25.
//

#include "ak47.h"

#include <algorithm>
#include <cmath>

bool Ak47::has_to_shoot(const float current_time) {
    return !shoots.empty() && (current_time - time_since_last_shot > miliseconds_per_shoot);
}

void Ak47::reload_gun() {
    if (current_ammo == 0 && reserve_ammo == 0) {
        throw NoAmmo();
    }
    const int max_ammo_to_reload = max_ammo - current_ammo;
    if (const int ammo_to_reload = std::min(reserve_ammo, max_ammo_to_reload); ammo_to_reload > 0) {
        reserve_ammo -= ammo_to_reload;
        current_ammo += ammo_to_reload;
    }
}


void Ak47::reset_shoots() {
    std::queue<std::pair<int, int>> empty;
    shoots.swap(empty);
}


void Ak47::shoot_gun(const Position final_position) {
    if (current_ammo == 0 || shoots.size() == current_ammo) {
        throw NoAmmo();
    }
    const auto pos = final_position.get_position();
    if (shoots.empty()) {
        const int bullets_to_shoot = std::min(bullets_per_burst, current_ammo);
        for (int i = 0; i < bullets_to_shoot; i++) {
            shoots.push(pos);
        }
    } else {
        if (shoots.size() == 1) {
            shoots.push(pos);
        }
    }
}


int Ak47::get_gun_price() { return ak_price; }


ShootResult Ak47::fire_gun(Map& map, Player& owner, const float current_time,
                           Position& current_position) {
    if (!has_to_shoot(current_time)) {
        return ShootResult{};
    }
    auto [x, y] = current_position.get_position();
    auto [final_x, final_y] = shoots.front();
    shoots.pop();
    current_ammo--;
    auto player_hit_o = map.trace_bullet_path(x, y, Position(final_x, final_y), owner);
    if (player_hit_o.has_value()) {
        const auto& player_hit = player_hit_o.value();
        auto [player_hit_x, player_hit_y] = player_hit->get_location();
        const double distance = std::sqrt(pow(x - player_hit_x, 2) + pow(y - player_hit_y, 2));
        const int damage_before_distance = min_dmg + std::rand() % (max_dmg - min_dmg + 1);
        const double damage = static_cast<double>(damage_before_distance) / 1 + (distance / 10);
        return ShootResult{static_cast<int>(damage), player_hit_o.value()};
    }
    return ShootResult{};
}
