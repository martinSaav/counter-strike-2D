//
// Created by matias on 09/06/25.
//

#include "bomb_encapsulator.h"

#include <utility>
#include <vector>

#include "player.h"

bool BombEncapsulator::has_to_shoot(float current_time) { return has_to_fire; }


ShootInfo BombEncapsulator::fire_gun(Map& map, Player& owner, const float current_time,
                                     Position& current_position) {
    has_to_fire = false;
    bomb.start_planting(current_time);
    owner.start_planting();
    std::vector<ShootResult> result;
    result.emplace_back();
    return ShootInfo{std::move(result)};
}


void BombEncapsulator::reset_shoots() {
    has_to_fire = false;
    bomb.stop_planting();
}


void BombEncapsulator::reload_gun() {}


void BombEncapsulator::shoot_gun(Position final_position, float time) { has_to_fire = true; }


bool BombEncapsulator::has_finished_planting(const float current_time) const {
    return bomb.has_finished_planting(current_time);
}


bool BombEncapsulator::is_planting() const { return bomb.is_currently_planting(); }
