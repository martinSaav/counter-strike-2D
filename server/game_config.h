//
// Created by matias on 17/06/25.
//

#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include "gun_config.h"


class GameConfig {
    int player_health;
    int number_of_rounds;
    int starting_money;
    int ct_amount;
    int tt_amount;
    GunConfig knife_config;
    GunConfig glock_config;
    GunConfig ak_config;
    int defuse_time;
    int time_to_plant;
    int bomb_dmg;
    int bomb_hitbox_height;
    int bomb_hitbox_width;
    int time_to_defuse;
    int round_winner_money;
    int round_loser_money;
    int buy_time;
    int bomb_time;
    int money_per_kill;
    int tiles_per_movement;
    int game_rate;
};


#endif  // GAMECONFIG_H
