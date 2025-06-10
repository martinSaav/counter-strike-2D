//
// Created by matias on 09/06/25.
//

#include "bomb.h"

void Bomb::start_planting(float time) {
    is_planting = true;
    time_when_started_to_plant = time;
}


bool Bomb::has_finished_planting(const float time) const {
    return time - time_when_started_to_plant > time_needed_to_plant && is_planting;
}


void Bomb::stop_planting() { is_planting = false; }
