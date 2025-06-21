//
// Created by matias on 08/06/25.
//

#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H
#include <map>
#include <stdexcept>

#include "game_config.h"
#include "stages.h"


struct RoundAlreadyFinished: public std::runtime_error {
    RoundAlreadyFinished(): std::runtime_error("Round has finished") {}
};

class GameClock {
    const int buy_stage_time;
    const int post_bomb_stage_time;
    const int after_round_stage_time;
    double current_time;
    double stage_time;
    Stages current_stage;
    bool bomb_exploded;
    bool has_round_finished;
    [[nodiscard]] std::map<Stages, double> get_stage_duration_map() const;

public:
    explicit GameClock(const GameConfig& config):
            buy_stage_time(config.buy_time),
            post_bomb_stage_time(config.bomb_time),
            after_round_stage_time(config.after_round_time),
            current_time(0),
            stage_time(0),
            current_stage(Stages::BuyStage),
            bomb_exploded(false),
            has_round_finished(false) {}
    [[nodiscard]] Stages getCurrentStage() const { return current_stage; }
    void advance(double dt);
    void reset();
    [[nodiscard]] bool has_bomb_exploded() const { return bomb_exploded; }
    void set_post_plant_stage();
    void set_after_round_stage();
    [[nodiscard]] double get_time() const { return current_time; }
    [[nodiscard]] double get_bomb_timer() const;
};


#endif  // GAME_CLOCK_H
