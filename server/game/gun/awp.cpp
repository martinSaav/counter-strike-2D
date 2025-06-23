//
// Created by matias on 18/06/25.
//

#include "awp.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "impact_info.h"
#include "map.h"

Awp::Awp(const GunConfig& awp_config):
        max_ammo(awp_config.max_ammo),
        max_dmg(awp_config.max_dmg),
        min_dmg(awp_config.min_dmg),
        gun_price(awp_config.gun_price),
        shoot_cooldown(awp_config.shoot_cooldown),
        base_precision(awp_config.base_precision),
        distance_precision_modifier(awp_config.distance_precision_modifier),
        current_ammo(awp_config.max_ammo),
        reserve_ammo(awp_config.starting_reserve_ammo),
        time_since_last_shot(0) {}


bool Awp::has_to_shoot(const float current_time) { return !shoots.empty(); }

void Awp::reload_gun() {
    if (current_ammo == 0 && reserve_ammo == 0) {
        throw NoAmmo();
    }
    const int max_ammo_to_reload = max_ammo - current_ammo;
    if (const int ammo_to_reload = std::min(reserve_ammo, max_ammo_to_reload); ammo_to_reload > 0) {
        reserve_ammo -= ammo_to_reload;
        current_ammo += ammo_to_reload;
    }
}


void Awp::reset_shoots() {
    std::queue<std::pair<int, int>> empty;
    shoots.swap(empty);
}


void Awp::shoot_gun(const Position final_position, float current_time) {
    if (current_ammo == 0 || shoots.size() == current_ammo) {
        throw NoAmmo();
    }
    if (current_time - time_since_last_shot < static_cast<float>(shoot_cooldown)) {
        return;
    }
    const auto pos = final_position.get_position();
    shoots.push(pos);
}


int Awp::get_gun_price() { return gun_price; }


bool Awp::calculate_if_bullet_hit(double distance) const {
    const double shoot_precision =
            std::max(0.0, base_precision * (1 - distance * distance_precision_modifier));
    const double roll = generate_random_number(0, 1);
    return roll < shoot_precision;
}


ShootInfo Awp::fire_gun(Map& map, Player& owner, const float current_time,
                        Position& current_position) {
    if (!has_to_shoot(current_time)) {
        throw DontHaveToShoot();
    }
    auto [x, y] = current_position.get_position();
    auto [final_x, final_y] = shoots.front();
    shoots.pop();
    current_ammo--;
    time_since_last_shot = current_time;
    const ImpactInfo impact = map.trace_bullet_path(x, y, Position(final_x, final_y), owner);
    std::vector<ShootResult> result;
    if (impact.impacted_player.has_value()) {
        const auto& player_hit = impact.impacted_player.value();
        const int damage = static_cast<int>(generate_random_number(min_dmg, max_dmg));
        auto [player_hit_x, player_hit_y] = player_hit->get_location();
        const double distance = std::sqrt(pow(x - player_hit_x, 2) + pow(y - player_hit_y, 2));
        if (calculate_if_bullet_hit(distance)) {
            result.emplace_back(damage, impact.impact_position, player_hit);
        } else {
            result.emplace_back(impact.impact_position);
        }
    } else {
        result.emplace_back(impact.impact_position);
    }
    return ShootInfo{std::move(result)};
}


void Awp::add_magazine() { reserve_ammo += max_ammo; }


void Awp::reset_time() { time_since_last_shot = 0; }
