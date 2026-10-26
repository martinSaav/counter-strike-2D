//
// Created by matias on 17/06/25.
//

#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include <string>
#include <vector>

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
    const GunConfig m3_config;
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
    // Mapa de la partida. En la configuracion global es el primero de la lista.
    const MapConfig map_config;
    const std::vector<MapConfig> maps;
    const int cone_angle;
    const int cone_opacity;
    const int round_time;
    GameConfig(int player_health, int number_of_rounds, int starting_money, int ct_amount,
               int tt_amount, int ammo_price, GunConfig&& knife_config, GunConfig&& glock_config,
               GunConfig&& ak_config, GunConfig&& awp_config, GunConfig&& m3_config,
               int defuse_time, int time_to_plant, int bomb_dmg, int round_winner_money,
               int round_loser_money, int buy_time, int bomb_time, int after_round_time,
               int money_per_kill, int tiles_per_movement, int game_rate, MapConfig&& map_config,
               int cone_angle, int cone_opacity, int round_time, std::vector<MapConfig> maps = {});
    // Copia de la configuracion que juega en otro mapa (cada partida elige el suyo).
    GameConfig(const GameConfig& base, const MapConfig& selected_map);
    // Mapa con ese nombre; si no existe, el primero.
    [[nodiscard]] const MapConfig& find_map(const std::string& name) const;
    [[nodiscard]] std::vector<std::string> get_map_names() const;
};


#endif  // GAMECONFIG_H
