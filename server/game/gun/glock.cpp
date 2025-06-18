//
// Created by matias on 03/06/25.
//

#include "glock.h"

#include <algorithm>
#include <cmath>

#include "map.h"
#include "player.h"

Glock::Glock(const GunConfig& glock_config):
        max_ammo(glock_config.max_ammo),
        gun_price(glock_config.gun_price),
        min_dmg(glock_config.min_dmg),
        max_dmg(glock_config.max_dmg),
        current_ammo(glock_config.max_ammo),
        reserve_ammo(glock_config.starting_reserve_ammo),
        has_to_fire(false) {}


int Glock::get_gun_price() { return gun_price; }

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
        throw DontHaveToShoot();
    }
    auto [x, y] = current_position.get_position();
    auto [final_x, final_y] = next_shoot;
    current_ammo--;
    has_to_fire = false;
    const ImpactInfo impact = map.trace_bullet_path(x, y, Position(final_x, final_y), owner);
    if (impact.impacted_player.has_value()) {
        const auto& player_hit = impact.impacted_player.value();
        auto [player_hit_x, player_hit_y] = player_hit->get_location();
        const double distance = std::sqrt(pow(x - player_hit_x, 2) + pow(y - player_hit_y, 2));
        const int damage_before_distance = min_dmg + std::rand() % (max_dmg - min_dmg + 1);
        const double damage = static_cast<double>(damage_before_distance) / (1 + distance / 10);
        return ShootResult{static_cast<int>(damage), impact.impact_position, player_hit};
    }
    return ShootResult{impact.impact_position};
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
