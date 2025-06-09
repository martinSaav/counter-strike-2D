//
// Created by matias on 06/06/25.
//

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <memory>
#include <vector>

#include "game_clock.h"
#include "player.h"
#include "time_information.h"


class GameManager {
    Map& map;
    GameClock& clock;
    bool bomb_planted;
    int bomb_x;
    int bomb_y;
    bool bomb_defused;
    bool round_won;
    Team last_winner;
    int current_round;
    int ct_rounds;
    int tt_rounds;

public:
    explicit GameManager(Map& map, GameClock& clock):
            map(map),
            clock(clock),
            bomb_planted(false),
            bomb_x(0),
            bomb_y(0),
            bomb_defused(false),
            round_won(false),
            last_winner(Team::Terrorists),
            current_round(1),
            ct_rounds(0),
            tt_rounds(0) {}

    void attack_player(const std::shared_ptr<Player>& attacked, Player& attacker, int damage);
    bool can_player_buy();
    bool can_player_move_or_shoot(const std::shared_ptr<Player>& player);
    void advance_round();
    void check_winning_cond(const std::vector<std::shared_ptr<Player>>& players);
    [[nodiscard]] double get_time() const { return clock.get_time(); }
    [[nodiscard]] TimeInformation get_time_information() const;
    [[nodiscard]] Map& get_map() const { return map; }
};


#endif  // GAME_MANAGER_H
