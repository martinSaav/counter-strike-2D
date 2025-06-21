//
// Created by matias on 17/06/25.
//

#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include "gun_config.h"
#include "map_config.h"


class GameConfig {
public:
    const int player_health;
    const int number_of_rounds;
    const int starting_money;
    const int ct_amount;
    const int tt_amount;
    const int ammo_price;
    const GunConfig knife_config;
    const GunConfig glock_config;
    const GunConfig ak_config;
    const GunConfig awp_config;
    const int defuse_time;
    const int time_to_plant;
    const int bomb_dmg;
    const int round_winner_money;
    const int round_loser_money;
    const int buy_time;
    const int bomb_time;
    const int after_round_time;
    const int money_per_kill;
    const int tiles_per_movement;
    const int game_rate;
    const MapConfig map_config;
    GameConfig(int player_health, int number_of_rounds, int starting_money, int ct_amount,
               int tt_amount, int ammo_price, GunConfig&& knife_config, GunConfig&& glock_config,
               GunConfig&& ak_config, GunConfig&& awp_config, int defuse_time, int time_to_plant,
               int bomb_dmg, int round_winner_money, int round_loser_money, int buy_time,
               int bomb_time, int after_round_time, int money_per_kill, int tiles_per_movement,
               int game_rate, MapConfig&& map_config);
};


#endif  // GAMECONFIG_H
