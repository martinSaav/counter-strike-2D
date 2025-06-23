//
// Created by matias on 21/06/25.
//

#include "m3.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "map.h"
#include "player.h"

M3::M3(const GunConfig& m3_config):
        max_ammo(m3_config.max_ammo),
        gun_price(m3_config.gun_price),
        min_dmg(m3_config.min_dmg),
        max_dmg(m3_config.max_dmg),
        degree(m3_config.angle),
        range(m3_config.range),
        shoot_cooldown(m3_config.shoot_cooldown),
        base_precision(m3_config.base_precision),
        distance_precision_modifier(m3_config.distance_precision_modifier),
        time_since_last_shot(0),
        current_ammo(max_ammo),
        reserve_ammo(m3_config.starting_reserve_ammo),
        has_to_fire(false) {}

int M3::get_gun_price() { return gun_price; }

void M3::shoot_gun(const Position final_position, float current_time) {
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

void M3::reset_shoots() { has_to_fire = false; }


bool M3::has_to_shoot(float current_time) { return has_to_fire; }


GunType M3::get_gun_type() { return type; }


WeaponInfo M3::get_weapon_name() { return WeaponInfo{Weapon::M3, current_ammo}; }


int M3::calculate_damage(const double distance) const {
    const int damage_before_distance = generate_random_number(min_dmg, max_dmg);
    const double damage = static_cast<double>(damage_before_distance) / (1 + distance / 2);
    return static_cast<int>(damage);
}


bool M3::calculate_if_bullet_hit(double distance) const {
    const double shoot_precision =
            std::max(0.0, base_precision * (1 - distance * distance_precision_modifier));
    const double roll = generate_random_number(0, 1);
    return roll < shoot_precision;
}


ShootInfo M3::fire_gun(Map& map, Player& owner, float current_time, Position& current_position) {
    if (!has_to_shoot(current_time)) {
        throw DontHaveToShoot();
    }
    time_since_last_shot = current_time;
    has_to_fire = false;
    current_ammo--;
    auto [x_center, y_center] = owner.get_center_coordinates();
    auto [final_x, final_y] = next_shoot;
    double direction_x = final_x - x_center;
    double direction_y = final_y - y_center;
    const std::pair<int, int> direction(direction_x, direction_y);
    const double module = sqrt(pow(direction_x, 2) + pow(direction_y, 2));
    direction_x = direction_x / (module);
    direction_y = direction_y / (module);
    const auto near_players = map.get_players_near_radio(x_center, y_center, range);
    std::vector<ShootResult> results;
    for (auto& player: near_players) {
        if (player->get_username() == owner.get_username()) {
            continue;
        }
        auto [other_x, other_y] = player->get_center_coordinates();
        const auto [attack_x, attack_y] = std::make_pair(other_x - x_center, other_y - y_center);
        const double mod =
                sqrt(attack_x * attack_x + attack_y * attack_y);  // tambien es la distancia
        if (mod == 0) {
            results.emplace_back(calculate_damage(mod), direction, player);
            continue;
        }
        const double cos = (attack_x * direction_x + attack_y * direction_y) / mod;
        if (double angle = acos(cos); angle < degree && calculate_if_bullet_hit(mod)) {
            results.emplace_back(calculate_damage(mod), direction, player);
        }
    }
    return ShootInfo{std::move(results)};
}


void M3::reload_gun() {
    if (current_ammo == 0 && reserve_ammo == 0) {
        throw NoAmmo();
    }
    const int max_ammo_to_reload = max_ammo - current_ammo;
    if (const int ammo_to_reload = std::min(reserve_ammo, max_ammo_to_reload); ammo_to_reload > 0) {
        reserve_ammo -= ammo_to_reload;
        current_ammo += ammo_to_reload;
    }
}


void M3::add_magazine() { reserve_ammo += max_ammo; }


void M3::reset_time() { time_since_last_shot = 0; }
