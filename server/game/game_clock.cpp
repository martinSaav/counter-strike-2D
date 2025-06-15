//
// Created by matias on 08/06/25.
//

#include "game_clock.h"


std::map<Stages, double> GameClock::get_stage_duration_map() {
    std::map<Stages, double> stage_duration_map = {{Stages::BuyStage, 45000},
                                                   {Stages::PrePlantStage, 0},
                                                   {Stages::PostPlantStage, 90000},
                                                   {Stages::AfterRoundStage, 10000}};
    return stage_duration_map;
}


void GameClock::advance(double dt) {
    current_time += dt;
    stage_time += dt;
    std::map<Stages, double> stage_duration_map = get_stage_duration_map();
    double duration = stage_duration_map[current_stage];
    if (stage_time >= duration && duration >= 0) {
        if (current_stage == Stages::BuyStage) {
            current_stage = Stages::PrePlantStage;
            stage_time -= duration;
        } else if (current_stage == Stages::PostPlantStage) {
            bomb_exploded = true;
            current_stage = Stages::AfterRoundStage;
            stage_time -= duration;
        } else if (current_stage == Stages::AfterRoundStage) {
            throw RoundAlreadyFinished();
        }
    }
}


void GameClock::reset() {
    current_stage = Stages::BuyStage;
    stage_time = 0;
    current_time = 0;
    bomb_exploded = false;
    has_round_finished = false;
}


void GameClock::set_post_plant_stage() {
    stage_time = 0;
    current_stage = Stages::PostPlantStage;
}

void GameClock::set_after_round_stage() {
    stage_time = 0;
    current_stage = Stages::AfterRoundStage;
}
