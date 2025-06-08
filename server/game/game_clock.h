//
// Created by matias on 08/06/25.
//

#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H
#include "stages.h"


class GameClock {
    double current_time;
    double stage_time;
    Stages current_stage;

public:
    GameClock(): current_time(0), stage_time(0), current_stage(Stages::BuyStage) {}
    Stages getCurrentStage() const { return current_stage; }
    void advance(double dt);
};


#endif  // GAME_CLOCK_H
