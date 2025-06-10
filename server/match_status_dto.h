//
// Created by matias on 15/05/25.
//

#ifndef MATCHSTATUSDTO_H
#define MATCHSTATUSDTO_H
#include <list>
#include <utility>
#include <vector>

#include "dto/game_state_update.h"
#include "server/player_dto.h"

class MatchStatusDTO {
public:
    const std::vector<PlayerDTO> players;
    const bool game_started;
    const bool game_ended;
    const int round;
    const float round_time;
    const bool round_started;
    const bool round_ended;
    const bool bomb_planted;
    const int bomb_x;
    const int bomb_y;
    const float bomb_timer;
    const Team round_winner;
    const Team game_winner;
    const std::list<DroppedWeapon> dropped_weapons;
    explicit MatchStatusDTO(std::vector<PlayerDTO>&& players, bool game_started, bool game_ended,
                            int round, float round_time, bool round_started, bool round_ended,
                            bool bomb_planted, int bomb_x, int bomb_y, float bomb_timer,
                            Team round_winner, Team game_winner,
                            std::list<DroppedWeapon> dropped_weapons):
            players(std::move(players)),
            game_started(game_started),
            game_ended(game_ended),
            round(round),
            round_time(round_time),
            round_started(round_started),
            round_ended(round_ended),
            bomb_planted(bomb_planted),
            bomb_x(bomb_x),
            bomb_y(bomb_y),
            bomb_timer(bomb_timer),
            round_winner(round_winner),
            game_winner(game_winner),
            dropped_weapons(std::move(dropped_weapons)) {}
};

#endif  // MATCHSTATUSDTO_H
