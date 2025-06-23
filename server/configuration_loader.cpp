//
// Created by matias on 17/06/25.
//

#include "configuration_loader.h"

#include <vector>

#include <yaml-cpp/yaml.h>

GunConfig ConfigurationLoader::get_knife_config(YAML::Node& knife_config) {
    const int min_dmg = knife_config["min_dmg"].as<int>();
    const int max_dmg = knife_config["max_dmg"].as<int>();
    const int range = knife_config["range"].as<int>();
    const auto angle = knife_config["angle"].as<double>();
    const int shoot_cooldown = knife_config["shoot_cooldown"].as<int>();
    return GunConfig{0, 0, min_dmg, max_dmg, 0, range, angle, shoot_cooldown, 0};
}


GunConfig ConfigurationLoader::get_glock_config(YAML::Node& glock_config) {
    const int max_ammo = glock_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = glock_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = glock_config["min_dmg"].as<int>();
    const int max_dmg = glock_config["max_dmg"].as<int>();
    const int gun_price = glock_config["gun_price"].as<int>();
    const int shoot_cooldown = glock_config["shoot_cooldown"].as<int>();
    return GunConfig{max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price, 0,
                     0,        shoot_cooldown,        0};
}


GunConfig ConfigurationLoader::get_ak_config(YAML::Node& ak_config) {
    const int max_ammo = ak_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = ak_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = ak_config["min_dmg"].as<int>();
    const int max_dmg = ak_config["max_dmg"].as<int>();
    const int gun_price = ak_config["gun_price"].as<int>();
    const int bullets_per_burst = ak_config["bullets_per_burst"].as<int>();
    const int shoot_cooldown = ak_config["shoot_cooldown"].as<int>();
    return GunConfig{max_ammo, starting_reserve_ammo, min_dmg,          max_dmg, gun_price, 0,
                     0,        shoot_cooldown,        bullets_per_burst};
}


GunConfig ConfigurationLoader::get_awp_config(YAML::Node& awp_config) {
    const int max_ammo = awp_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = awp_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = awp_config["min_dmg"].as<int>();
    const int max_dmg = awp_config["max_dmg"].as<int>();
    const int gun_price = awp_config["gun_price"].as<int>();
    const int shoot_cooldown = awp_config["shoot_cooldown"].as<int>();
    return GunConfig{max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price, 0,
                     0,        shoot_cooldown,        0};
}


GunConfig ConfigurationLoader::get_m3_config(YAML::Node& m3_config) {
    const int max_ammo = m3_config["max_ammo"].as<int>();
    const int starting_reserve_ammo = m3_config["starting_reserve_ammo"].as<int>();
    const int min_dmg = m3_config["min_dmg"].as<int>();
    const int max_dmg = m3_config["max_dmg"].as<int>();
    const int gun_price = m3_config["gun_price"].as<int>();
    const int range = m3_config["range"].as<int>();
    const auto angle = m3_config["angle"].as<double>();
    const int shoot_cooldown = m3_config["shoot_cooldown"].as<int>();
    return GunConfig{max_ammo, starting_reserve_ammo, min_dmg, max_dmg, gun_price, range,
                     angle,    shoot_cooldown,        0};
}


Site ConfigurationLoader::get_site_config(YAML::Node& site_config) {
    int x = site_config["x"].as<int>();
    int y = site_config["y"].as<int>();
    int height = site_config["height"].as<int>();
    int width = site_config["width"].as<int>();
    YAML::Node spawns = site_config["spawns"];
    std::vector<std::pair<int, int>> spawn_positions;
    for (auto spawn: spawns) {
        int spawn_x = spawn["x"].as<int>();
        int spawn_y = spawn["y"].as<int>();
        spawn_positions.emplace_back(spawn_x, spawn_y);
    }
    return Site{x, y, width, height, (std::move(spawn_positions))};
}


MapConfig ConfigurationLoader::get_map_config(YAML::Node& map_config) {
    auto map_name = map_config["map_name"].as<std::string>();
    auto map_height = map_config["map_height"].as<int>();
    auto map_width = map_config["map_width"].as<int>();
    YAML::Node structures = map_config["structures"];
    std::vector<Structure> structures_v;
    for (auto structure: structures) {
        int x = structure["x"].as<int>();
        int y = structure["y"].as<int>();
        int width = structure["width"].as<int>();
        int height = structure["height"].as<int>();
        Structure structure_v(height, width, Position{x, y});
        structures_v.push_back(structure_v);
    }
    YAML::Node bomb_site = map_config["bomb_site"];
    YAML::Node tt_site = map_config["tt_site"];
    YAML::Node ct_site = map_config["ct_site"];
    int bomb_site_x = bomb_site["x"].as<int>();
    int bomb_site_y = bomb_site["y"].as<int>();
    int bomb_site_height = bomb_site["height"].as<int>();
    int bomb_site_width = bomb_site["width"].as<int>();
    BombSite bombsite(bomb_site_height, bomb_site_width, bomb_site_x, bomb_site_y);
    Site tt = get_site_config(tt_site);
    Site ct = get_site_config(ct_site);
    return MapConfig{std::move(map_name), map_width,    map_height, structures_v, bombsite,
                     std::move(ct),       std::move(tt)};
}


GameConfig ConfigurationLoader::load_configuration() const {
    YAML::Node config = YAML::LoadFile(filename);
    int player_health = config["player_health"].as<int>();
    int number_of_rounds = config["number_of_rounds"].as<int>();
    int starting_money = config["starting_money"].as<int>();
    int ct_amount = config["ct_amount"].as<int>();
    int tt_amount = config["tt_amount"].as<int>();
    int ammo_price = config["ammo_price"].as<int>();
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
    YAML::Node m3_config = config["m3"];
    GunConfig knife = get_knife_config(knife_config);
    GunConfig glock = get_glock_config(glock_config);
    GunConfig ak = get_ak_config(ak_config);
    GunConfig awp = get_awp_config(awp_config);
    GunConfig m3 = get_m3_config(m3_config);
    YAML::Node map_config = config["map_config"];
    MapConfig map = get_map_config(map_config);
    return GameConfig{player_health,      number_of_rounds, starting_money,     ct_amount,
                      tt_amount,          ammo_price,       std::move(knife),   std::move(glock),
                      std::move(ak),      std::move(awp),   std::move(m3),      defuse_time,
                      time_to_plant,      bomb_dmg,         round_winner_money, round_loser_money,
                      buy_time,           bomb_time,        after_round_time,   money_per_kill,
                      tiles_per_movement, game_rate,        std::move(map)};
}
