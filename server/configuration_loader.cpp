//
// Created by matias on 17/06/25.
//

#include "configuration_loader.h"

#include <yaml-cpp/yaml.h>


GunConfig ConfigurationLoader::get_knife_config(YAML::Node& knife_config) {
    const int min_dmg = knife_config["min_dmg"].as<int>();
    const int max_dmg = knife_config["max_dmg"].as<int>();
    const int range = knife_config["range"].as<int>();
    const auto angle = knife_config["angle"].as<double>();
    return GunConfig{min_dmg, max_dmg, range, angle};
}


GunConfig ConfigurationLoader::get_glock_config(YAML::Node& glock_config) {
    const int max_ammo = glock_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = glock_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = glock_config["min_dmg"].as<int>();
    const int max_dmg = glock_config["max_dmg"].as<int>();
    const int gun_price = glock_config["gun_price"].as<int>();
    return GunConfig{max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price};
}


GunConfig ConfigurationLoader::get_ak_config(YAML::Node& ak_config) {
    const int max_ammo = ak_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = ak_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = ak_config["min_dmg"].as<int>();
    const int max_dmg = ak_config["max_dmg"].as<int>();
    const int gun_price = ak_config["gun_price"].as<int>();
    const int bullets_per_burst = ak_config["bullets_per_burst"].as<int>();
    const int shoot_cooldown = ak_config["shoot_cooldown"].as<int>();
    return GunConfig{max_ammo,  starting_reserve_ammo, min_dmg,       max_dmg,
                     gun_price, bullets_per_burst,     shoot_cooldown};
}


GunConfig ConfigurationLoader::get_awp_config(YAML::Node& awp_config) {
    const int max_ammo = awp_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = awp_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = awp_config["min_dmg"].as<int>();
    const int max_dmg = awp_config["max_dmg"].as<int>();
    const int gun_price = awp_config["gun_price"].as<int>();
    const int shoot_cooldown = awp_config["shoot_cooldown"].as<int>();
    return GunConfig{max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price, shoot_cooldown};
}


GameConfig ConfigurationLoader::load_configuration() const {
    YAML::Node config = YAML::LoadFile(filename);
    int player_health = config["player_health"].as<int>();
    int number_of_rounds = config["number_of_rounds"].as<int>();
    int starting_money = config["starting_money"].as<int>();
    int ct_amount = config["ct_amount"].as<int>();
    int tt_amount = config["tt_amount"].as<int>();
    int defuse_time = config["defuse_time"].as<int>();
    int time_to_plant = config["time_to_plant"].as<int>();
    int bomb_dmg = config["bomb_dmg"].as<int>();
    int round_winner_money = config["round_winner_money"].as<int>();
    int round_loser_money = config["round_loser_money"].as<int>();
    int buy_time = config["buy_time"].as<int>();
    int bomb_time = config["bomb_time"].as<int>();
    int after_round_time = config["after_round_time"].as<int>();
    int money_per_kill = config["money_per_kill"].as<int>();
    int tiles_per_movement = config["tiles_per_movement"].as<int>();
    int game_rate = config["game_rate"].as<int>();
    YAML::Node knife_config = config["knife"];
    YAML::Node ak_config = config["ak"];
    YAML::Node glock_config = config["glock"];
    YAML::Node awp_config = config["awp"];
    GunConfig knife = get_knife_config(knife_config);
    GunConfig glock = get_glock_config(glock_config);
    GunConfig ak = get_ak_config(ak_config);
    GunConfig awp = get_awp_config(awp_config);

    return GameConfig{player_health,      number_of_rounds,  starting_money,     ct_amount,
                      tt_amount,          std::move(knife),  std::move(glock),   std::move(ak),
                      std::move(awp),     defuse_time,       time_to_plant,      bomb_dmg,
                      round_winner_money, round_loser_money, buy_time,           bomb_time,
                      after_round_time,   money_per_kill,    tiles_per_movement, game_rate};
}
