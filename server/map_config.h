//
// Created by matias on 19/06/25.
//

#ifndef MAP_CONFIG_H
#define MAP_CONFIG_H

#include <string>
#include <utility>
#include <vector>

#include "bomb_site.h"
#include "site.h"
#include "structure.h"
#include "weapon.h"

class MapConfig {
public:
    const std::string map_name;
    const int map_width;
    const int map_height;
    const std::vector<Structure> structures;
    const BombSite bombsite;
    const Site ct_site;
    const Site tt_site;
    const std::vector<std::pair<Position, Weapon>> dropped_weapons;
    MapConfig(std::string&& map_name, const int map_width, const int map_height,
              const std::vector<Structure>& structures, const BombSite& bombsite, Site&& ct_site,
              Site&& tt_site, std::vector<std::pair<Position, Weapon>> dropped_weapons):
            map_name(std::move(map_name)),
            map_width(map_width),
            map_height(map_height),
            structures(structures),
            bombsite(bombsite),
            ct_site(std::move(ct_site)),
            tt_site(std::move(tt_site)),
            dropped_weapons(std::move(dropped_weapons)) {}
};


#endif  // MAP_CONFIG_H
