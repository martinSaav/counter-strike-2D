//
// Created by matias on 17/06/25.
//

#include "game_config.h"

#include <utility>

GameConfig::GameConfig(int player_health, int number_of_rounds, int starting_money, int ct_amount,
                       int tt_amount, GunConfig&& knife_config, GunConfig&& glock_config,
                       GunConfig&& ak_config, int defuse_time, int time_to_plant, int bomb_dmg,
                       int round_winner_money, int round_loser_money, int buy_time, int bomb_time,
                       int after_round_time, int money_per_kill, int tiles_per_movement,
                       int game_rate):
        player_health(player_health),
        number_of_rounds(number_of_rounds),
        starting_money(starting_money),
        ct_amount(ct_amount),
        tt_amount(tt_amount),
        knife_config(std::move(knife_config)),
        glock_config(std::move(glock_config)),
        ak_config(std::move(ak_config)),
        defuse_time(defuse_time),
        time_to_plant(time_to_plant),
        bomb_dmg(bomb_dmg),
        round_winner_money(round_winner_money),
        round_loser_money(round_loser_money),
        buy_time(buy_time),
        bomb_time(bomb_time),
        after_round_time(after_round_time),
        money_per_kill(money_per_kill),
        tiles_per_movement(tiles_per_movement),
        game_rate(game_rate) {}
