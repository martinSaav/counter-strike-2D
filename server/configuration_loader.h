//
// Created by matias on 17/06/25.
//

#ifndef CONFIGURATION_LOADER_H
#define CONFIGURATION_LOADER_H
#include <string>
#include <utility>
#include <vector>

#include "yaml-cpp/node/node.h"

#include "game_config.h"


class ConfigurationLoader {
    std::string filename;

    static GunConfig get_knife_config(YAML::Node& knife_config);

    static GunConfig get_glock_config(YAML::Node& glock_config);

    static GunConfig get_ak_config(YAML::Node& ak_config);

    static GunConfig get_awp_config(YAML::Node& awp_config);

    static GunConfig get_m3_config(YAML::Node& m3_config);

    static Site get_site_config(YAML::Node& site_config);

    static MapConfig get_map_config(YAML::Node& map_config);

    static std::vector<std::pair<Position, Weapon>> get_dropped_weapons(
            const YAML::Node& dropped_weapons_config);

public:
    explicit ConfigurationLoader(std::string&& filename): filename(std::move(filename)) {}
    [[nodiscard]] GameConfig load_configuration() const;
};


#endif  // CONFIGURATION_LOADER_H
