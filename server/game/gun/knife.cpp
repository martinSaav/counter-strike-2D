//
// Created by matias on 06/06/25.
//

#include "knife.h"

#include <cmath>
#include <memory>

#include "map.h"
#include "player.h"


void Knife::shoot_gun(Position final_position) {
    const auto pos = final_position.get_position();
    next_shoot = pos;
    has_to_fire = true;
}

void Knife::reset_shoots() { has_to_fire = false; }


bool Knife::has_to_shoot(float current_time) { return has_to_fire; }


int Knife::calculate_damage(const double distance) const {
    const int damage_before_distance = min_dmg + std::rand() % (max_dmg - min_dmg + 1);
    const double damage = static_cast<double>(damage_before_distance) / (1 + distance / 10);
    return static_cast<int>(damage);
}


ShootResult Knife::fire_gun(Map& map, Player& owner, float current_time,
                            Position& current_position) {
    if (!has_to_shoot(current_time)) {
        throw DontHaveToShoot();
    }
    has_to_fire = false;
    auto [x_center, y_center] = owner.get_center_coordinates();
    auto [final_x, final_y] = next_shoot;
    double direction_x = final_x - x_center;
    double direction_y = final_y - y_center;
    const std::pair<int, int> direction(direction_x, direction_y);
    const double module = sqrt(pow(direction_x, 2) + pow(direction_y, 2));
    direction_x = direction_x / (module);
    direction_y = direction_y / (module);
    const auto near_players = map.get_players_near_radio(x_center, y_center, knife_range);
    double nearest_player_distance = -1;
    std::shared_ptr<Player> nearest_player = nullptr;
    for (auto& player: near_players) {
        if (player->get_username() == owner.get_username()) {
            continue;
        }
        auto [other_x, other_y] = player->get_center_coordinates();
        const auto [attack_x, attack_y] = std::make_pair(other_x - x_center, other_y - y_center);
        const double mod =
                sqrt(attack_x * attack_x + attack_y * attack_y);  // tambien es la distancia
        if (mod == 0) {
            return ShootResult{calculate_damage(0), direction, player};
        }
        const double cos = (attack_x * direction_x + attack_y * direction_y) / mod;
        if (double angle = acos(cos); angle < max_degree) {
            if (nearest_player_distance > mod || nearest_player_distance == -1) {
                nearest_player_distance = mod;
                nearest_player = player;
            }
        }
    }
    if (nearest_player != nullptr) {
        return ShootResult{calculate_damage(nearest_player_distance), direction, nearest_player};
    }
    return ShootResult{direction};
}

void Knife::reload_gun() {}
