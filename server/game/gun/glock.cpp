//
// Created by matias on 03/06/25.
//

#include "glock.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "map.h"
#include "player.h"

Glock::Glock(const GunConfig& glock_config):
        max_ammo(glock_config.max_ammo),
        gun_price(glock_config.gun_price),
        min_dmg(glock_config.min_dmg),
        max_dmg(glock_config.max_dmg),
        shoot_cooldown(glock_config.shoot_cooldown),
        base_precision(glock_config.base_precision),
        distance_precision_modifier(glock_config.distance_precision_modifier),
        current_ammo(glock_config.max_ammo),
        reserve_ammo(glock_config.starting_reserve_ammo),
        time_since_last_shot(0),
        has_to_fire(false) {}


int Glock::get_gun_price() { return gun_price; }

void Glock::shoot_gun(Position final_position, float current_time) {
    if (current_ammo == 0) {
        throw NoAmmo();
    }
    if (current_time - time_since_last_shot < static_cast<float>(shoot_cooldown)) {
        return;
    }
    const auto pos = final_position.get_position();
    next_shoot = pos;
    has_to_fire = true;
}

void Glock::reset_shoots() { has_to_fire = false; }


bool Glock::has_to_shoot(float current_time) { return has_to_fire; }


bool Glock::calculate_if_bullet_hit(double distance) const {
    const double shoot_precision =
            std::max(0.0, base_precision * (1 - distance * distance_precision_modifier));
    const double roll = generate_random_number(0, 1);
    return roll < shoot_precision;
}


ShootInfo Glock::fire_gun(Map& map, Player& owner, float current_time, Position& current_position) {
    if (!has_to_shoot(current_time)) {
        throw DontHaveToShoot();
    }
    time_since_last_shot = current_time;
    auto [x, y] = current_position.get_position();
    auto [final_x, final_y] = next_shoot;
    current_ammo--;
    has_to_fire = false;
    const ImpactInfo impact = map.trace_bullet_path(x, y, Position(final_x, final_y), owner);
    std::vector<ShootResult> result;
    if (impact.impacted_player.has_value()) {
        const auto& player_hit = impact.impacted_player.value();
        auto [player_hit_x, player_hit_y] = player_hit->get_location();
        const double distance = std::sqrt(pow(x - player_hit_x, 2) + pow(y - player_hit_y, 2));
        const int damage_before_distance =
                static_cast<int>(generate_random_number(min_dmg, max_dmg));
        const double damage = static_cast<double>(damage_before_distance) / (1 + distance / 10);
        if (calculate_if_bullet_hit(distance)) {
            result.emplace_back(static_cast<int>(damage), impact.impact_position, player_hit);
        } else {
            result.emplace_back(impact.impact_position);
        }
    } else {
        result.emplace_back(impact.impact_position);
    }
    return ShootInfo{std::move(result)};
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


void Glock::add_magazine() { reserve_ammo += max_ammo; }
