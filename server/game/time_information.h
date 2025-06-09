//
// Created by matias on 08/06/25.
//

#ifndef TIME_INFORMATION_H
#define TIME_INFORMATION_H
#include "dto/game_state_update.h"


class TimeInformation {
public:
    const int round;
    const float round_time;
    const bool round_started;
    const bool round_ended;
    const bool bomb_planted;
    const int bomb_x;
    const int bomb_y;
    const float bomb_timer;
    const Team round_winner;
    TimeInformation(int round, float round_time, bool round_started, bool round_ended,
                    bool bomb_planted, int bomb_x, int bomb_y, float bomb_timer, Team round_winner):
            round(round),
            round_time(round_time),
            round_started(round_started),
            round_ended(round_ended),
            bomb_planted(bomb_planted),
            bomb_x(bomb_x),
            bomb_y(bomb_y),
            bomb_timer(bomb_timer),
            round_winner(round_winner) {}
};


#endif  // TIME_INFORMATION_H
