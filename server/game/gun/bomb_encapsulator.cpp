//
// Created by matias on 09/06/25.
//

#include "bomb_encapsulator.h"

#include "player.h"


bool BombEncapsulator::has_to_shoot(float current_time) { return has_to_fire; }


ShootResult BombEncapsulator::fire_gun(Map& map, Player& owner, const float current_time,
                                       Position& current_position) {
    bomb.start_planting(current_time);
    owner.start_planting();
    return ShootResult{};
}


void BombEncapsulator::reset_shoots() {
    has_to_fire = false;
    bomb.stop_planting();
}


void BombEncapsulator::reload_gun() {}


void BombEncapsulator::shoot_gun(Position final_position) { has_to_fire = true; }


bool BombEncapsulator::has_finished_planting(const float current_time) const {
    return bomb.has_finished_planting(current_time);
}


bool BombEncapsulator::is_planting() const { return bomb.is_currently_planting(); }
