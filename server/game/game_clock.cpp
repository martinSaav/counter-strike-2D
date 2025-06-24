//
// Created by matias on 08/06/25.
//

#include "game_clock.h"

#include <algorithm>

std::map<Stages, double> GameClock::get_stage_duration_map() const {
    std::map<Stages, double> stage_duration_map = {
            {Stages::BuyStage, buy_stage_time},
            {Stages::PrePlantStage, 0},
            {Stages::PostPlantStage, post_bomb_stage_time},
            {Stages::AfterRoundStage, after_round_stage_time}};
    return stage_duration_map;
}


void GameClock::advance(const double dt) {
    current_time += dt;
    stage_time += dt;
    if (bomb_planted && !bomb_exploded && !bomb_defused) {
        bomb_timer += dt;
    }
    std::map<Stages, double> stage_duration_map = get_stage_duration_map();
    double duration = stage_duration_map[current_stage];
    if (stage_time >= duration && duration >= 0) {
        if (current_stage == Stages::BuyStage) {
            current_stage = Stages::PrePlantStage;
            stage_time -= duration;
        } else if (current_stage == Stages::PostPlantStage) {
            bomb_exploded = true;
            bomb_timer = duration;
            current_stage = Stages::AfterRoundStage;
            stage_time -= duration;
        } else if (current_stage == Stages::AfterRoundStage) {
            throw RoundAlreadyFinished();
        }
    }
    if (bomb_timer > stage_duration_map[Stages::PostPlantStage] &&
        current_stage != Stages::PostPlantStage) {
        bomb_exploded = true;
    }
}


void GameClock::reset() {
    current_stage = Stages::BuyStage;
    stage_time = 0;
    current_time = 0;
    bomb_timer = 0;
    bomb_planted = false;
    bomb_exploded = false;
    bomb_defused = false;
    has_round_finished = false;
}


void GameClock::set_post_plant_stage() {
    stage_time = 0;
    current_stage = Stages::PostPlantStage;
    bomb_planted = true;
}

void GameClock::set_after_round_stage() {
    stage_time = 0;
    current_stage = Stages::AfterRoundStage;
}


void GameClock::defuse_bomb() {
    stage_time = 0;
    current_stage = Stages::AfterRoundStage;
    bomb_defused = true;
}


double GameClock::get_bomb_timer() const {
    if (bomb_timer != 0) {
        return std::max(post_bomb_stage_time - bomb_timer, static_cast<double>(0));
    }
    return 0;
}
