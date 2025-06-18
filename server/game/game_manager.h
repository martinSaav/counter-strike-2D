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
    GameConfig& config;
    const int time_to_defuse;
    const int round_winner_money;
    const int round_loser_money;
    const int bomb_dmg;
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
    explicit GameManager(Map& map, GameClock& clock, GameConfig& config):
            map(map),
            clock(clock),
            config(config),
            time_to_defuse(config.defuse_time),
            round_winner_money(config.round_winner_money),
            round_loser_money(config.round_loser_money),
            bomb_dmg(config.bomb_dmg),
            bomb_planted(false),
            bomb_x(0),
            bomb_y(0),
            bomb_defused(false),
            round_won(false),
            last_winner(Team::Terrorists),
            current_round(1),
            ct_rounds(0),
            tt_rounds(0) {}

    void attack_player(const std::shared_ptr<Player>& attacked, Player& attacker, int damage) const;
    bool can_player_buy() const;
    bool can_player_move_or_shoot(const std::shared_ptr<Player>& player) const;
    void advance_round(const std::vector<std::shared_ptr<Player>>& players);
    void check_winning_cond(const std::vector<std::shared_ptr<Player>>& players);
    [[nodiscard]] double get_time() const { return clock.get_time(); }
    [[nodiscard]] TimeInformation get_time_information() const;
    [[nodiscard]] Map& get_map() const { return map; }
    void plant_bomb(int x, int y);
    [[nodiscard]] bool is_bomb_planted() const { return bomb_planted; }
    [[nodiscard]] bool can_plant_bomb(int x, int y) const;
    void explode_bomb(const std::vector<std::shared_ptr<Player>>& players) const;
    void start_defusing(const std::shared_ptr<Player>& player) const;
    void has_finished_defusing(const std::shared_ptr<Player>& player);
    void drop_bomb(Player& player, std::unique_ptr<BombEncapsulator> bomb) const;
    void drop_weapon(Player& player, std::unique_ptr<Gun> gun) const;
    void pick_weapon(const std::shared_ptr<Player>& player) const;
    [[nodiscard]] bool has_to_switch_sides() const;
    [[nodiscard]] bool has_ended() const;
    [[nodiscard]] Team get_match_winner() const;
    void give_bomb_to_random_player(const std::vector<std::shared_ptr<Player>>& players) const;
    void switch_sides();
};


#endif  // GAME_MANAGER_H
