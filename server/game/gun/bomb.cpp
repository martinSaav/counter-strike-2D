//
// Created by matias on 09/06/25.
//

#include "bomb.h"

#include <algorithm>

void Bomb::start_planting(const float time) {
    is_planting = true;
    time_when_started_to_plant = time;
}


bool Bomb::has_finished_planting(const float time) const {
    return time - time_when_started_to_plant > time_needed_to_plant && is_planting;
}


void Bomb::stop_planting() { is_planting = false; }


float Bomb::get_planting_progress(const float time) const {
    if (!is_planting || time_needed_to_plant <= 0) {
        return 0;
    }
    return std::clamp(
            (time - time_when_started_to_plant) / static_cast<float>(time_needed_to_plant), 0.0f,
            1.0f);
}
