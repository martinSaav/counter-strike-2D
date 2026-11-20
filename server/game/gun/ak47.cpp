//
// Created by matias on 03/06/25.
//

#include "ak47.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "map.h"
#include "player.h"

namespace {
// Marca de "nunca disparo": cualquier tiempo de la ronda ya supera los cooldowns
constexpr float never = -1e6f;
// El reloj avanza de a 1000/30 ms: tres pasos suman 99,99 ms y sin esta tolerancia un cooldown de
// 100 ms se iria al cuarto paso
constexpr float tick_tolerance = 1.0f;
}  // namespace


Ak47::Ak47(const GunConfig& ak_config):
        max_ammo(ak_config.max_ammo),
        bullets_per_burst(ak_config.bullets_per_burst),
        max_dmg(ak_config.max_dmg),
        min_dmg(ak_config.min_dmg),
        gun_price(ak_config.gun_price),
        miliseconds_per_shoot(ak_config.shoot_cooldown),
        miliseconds_between_bursts(ak_config.burst_cooldown),
        base_precision(ak_config.base_precision),
        distance_precision_modifier(ak_config.distance_precision_modifier),
        current_ammo(ak_config.max_ammo),
        reserve_ammo(ak_config.starting_reserve_ammo),
        time_since_last_shot(never),
        time_since_burst_ended(never),
        bullets_left_in_burst(0),
        target(0, 0) {}


bool Ak47::has_to_shoot(const float current_time) {
    const float since_last_shot = current_time - time_since_last_shot + tick_tolerance;
    return bullets_left_in_burst > 0 &&
           since_last_shot >= static_cast<float>(miliseconds_per_shoot);
}

bool Ak47::can_reload() { return reserve_ammo != 0 && current_ammo < max_ammo; }


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


void Ak47::reset_shoots() { bullets_left_in_burst = 0; }


// El cliente manda un disparo por frame mientras se mantiene el click: cada llamada actualiza la
// mira de la rafaga en curso, y si no hay ninguna y ya paso la pausa, empieza otra.
void Ak47::shoot_gun(const Position final_position, const float current_time) {
    if (current_ammo == 0) {
        throw NoAmmo();
    }
    target = final_position.get_position();
    if (bullets_left_in_burst > 0) {
        return;
    }
    if (current_time - time_since_burst_ended + tick_tolerance >=
        static_cast<float>(miliseconds_between_bursts)) {
        bullets_left_in_burst = std::min(bullets_per_burst, current_ammo);
    }
}


int Ak47::get_gun_price() { return gun_price; }


bool Ak47::calculate_if_bullet_hit(double distance) const {
    const double shoot_precision =
            std::max(0.0, base_precision * (1 - distance * distance_precision_modifier));
    const double roll = generate_random_number(0, 1);
    return roll < shoot_precision;
}


ShootInfo Ak47::fire_gun(Map& map, Player& owner, const float current_time,
                         Position& current_position) {
    if (!has_to_shoot(current_time)) {
        throw DontHaveToShoot();
    }
    auto [x, y] = current_position.get_position();
    auto [final_x, final_y] = target;
    current_ammo--;
    bullets_left_in_burst--;
    time_since_last_shot = current_time;
    if (bullets_left_in_burst == 0) {
        time_since_burst_ended = current_time;
    }
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


void Ak47::add_magazine() { reserve_ammo += max_ammo; }


void Ak47::reset_time() {
    time_since_last_shot = never;
    time_since_burst_ended = never;
}
