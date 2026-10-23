//
// Created by matias on 17/06/25.
//

#include "game_config.h"

#include <utility>

GameConfig::GameConfig(int player_health, int number_of_rounds, int starting_money, int ct_amount,
                       int tt_amount, int ammo_price, GunConfig&& knife_config,
                       GunConfig&& glock_config, GunConfig&& ak_config, GunConfig&& awp_config,
                       GunConfig&& m3_config, int defuse_time, int time_to_plant, int bomb_dmg,
                       int round_winner_money, int round_loser_money, int buy_time, int bomb_time,
                       int after_round_time, int money_per_kill, int tiles_per_movement,
                       int game_rate, MapConfig&& map_config, int cone_angle, int cone_opacity,
                       int round_time, std::vector<MapConfig> maps):
        player_health(player_health),
        number_of_rounds(number_of_rounds),
        starting_money(starting_money),
        ct_amount(ct_amount),
        tt_amount(tt_amount),
        ammo_price(ammo_price),
        knife_config(std::move(knife_config)),
        glock_config(std::move(glock_config)),
        ak_config(std::move(ak_config)),
        awp_config(std::move(awp_config)),
        m3_config(std::move(m3_config)),
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
        game_rate(game_rate),
        map_config(std::move(map_config)),
        maps(std::move(maps)),
        cone_angle(cone_angle),
        cone_opacity(cone_opacity),
        round_time(round_time) {}


GameConfig::GameConfig(const GameConfig& base, const MapConfig& selected_map):
        player_health(base.player_health),
        number_of_rounds(base.number_of_rounds),
        starting_money(base.starting_money),
        ct_amount(base.ct_amount),
        tt_amount(base.tt_amount),
        ammo_price(base.ammo_price),
        knife_config(base.knife_config),
        glock_config(base.glock_config),
        ak_config(base.ak_config),
        awp_config(base.awp_config),
        m3_config(base.m3_config),
        defuse_time(base.defuse_time),
        time_to_plant(base.time_to_plant),
        bomb_dmg(base.bomb_dmg),
        round_winner_money(base.round_winner_money),
        round_loser_money(base.round_loser_money),
        buy_time(base.buy_time),
        bomb_time(base.bomb_time),
        after_round_time(base.after_round_time),
        money_per_kill(base.money_per_kill),
        tiles_per_movement(base.tiles_per_movement),
        game_rate(base.game_rate),
        map_config(selected_map),
        cone_angle(base.cone_angle),
        cone_opacity(base.cone_opacity),
        round_time(base.round_time) {}


const MapConfig& GameConfig::find_map(const std::string& name) const {
    for (const auto& map: maps) {
        if (map.map_name == name) {
            return map;
        }
    }
    return maps.empty() ? map_config : maps.front();
}


std::vector<std::string> GameConfig::get_map_names() const {
    std::vector<std::string> names;
    for (const auto& map: maps) {
        names.push_back(map.map_name);
    }
    if (names.empty()) {
        names.push_back(map_config.map_name);
    }
    return names;
}
