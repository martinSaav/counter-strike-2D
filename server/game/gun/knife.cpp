//
// Created by matias on 06/06/25.
//

#include "knife.h"

#include <cmath>
#include <memory>

void Knife::shoot_gun(Position final_position) {
    const auto pos = final_position.get_position();
    next_shoot = pos;
    has_to_fire = true;
}

void Knife::reset_shoots() { has_to_fire = false; }


bool Knife::has_to_shoot(float current_time) { return has_to_fire; }


int Knife::calculate_damage(const double distance) {
    const int damage_before_distance = min_dmg + std::rand() % (max_dmg - min_dmg + 1);
    const double damage = static_cast<double>(damage_before_distance) / 1 + (distance / 10);
    return static_cast<int>(damage);
}


ShootResult Knife::fire_gun(Map& map, Player& owner, float current_time,
                            Position& current_position) {
    if (!has_to_shoot(current_time)) {
        return ShootResult{};
    }
    has_to_fire = false;
    auto [x_center, y_center] = owner.get_center_coordinates();
    auto [final_x, final_y] = next_shoot;
    double direction_x = final_x - x_center;
    double direction_y = final_y - y_center;
    double module = sqrt(pow(direction_x, 2) + pow(direction_y, 2));
    direction_x = direction_x / (module);
    direction_y = direction_y / (module);
    auto near_players = map.get_players_near_radio(x_center, y_center, knife_range);
    double nearest_player_distance = -1;
    std::shared_ptr<Player> nearest_player = nullptr;
    for (auto& player: near_players) {
        if (*player == owner) {
            continue;
        }
        auto [other_x, other_y] = player->get_center_coordinates();
        std::pair<double, double> attack_vector =
                std::make_pair(other_x - x_center, other_y - y_center);
        double mod = sqrt(attack_vector.first * attack_vector.first +
                          attack_vector.second * attack_vector.second);  // tambien es la distancia
        if (mod == 0) {
            return ShootResult{calculate_damage(0), player};
        }
        double cos = (attack_vector.first * direction_x + attack_vector.second * direction_y) / mod;
        double angle = acos(cos);
        if (angle < max_degree) {
            if (nearest_player_distance > mod || nearest_player_distance == -1) {
                nearest_player_distance = mod;
                nearest_player = player;
            }
        }
    }
    if (nearest_player != nullptr) {
        return ShootResult{calculate_damage(nearest_player_distance), nearest_player};
    }
    return ShootResult{};
}

void Knife::reload_gun() {}
