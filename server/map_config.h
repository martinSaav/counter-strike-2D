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
    // Zonas donde los terroristas pueden plantar (al menos una)
    const std::vector<BombSite> bomb_sites;
    const Site ct_site;
    const Site tt_site;
    const std::vector<std::pair<Position, Weapon>> dropped_weapons;
    // Estilo visual del mapa: el cliente busca sus texturas en maps/<theme>/.
    const std::string theme;
    MapConfig(std::string&& map_name, const int map_width, const int map_height,
              const std::vector<Structure>& structures, std::vector<BombSite> bomb_sites,
              Site&& ct_site, Site&& tt_site,
              std::vector<std::pair<Position, Weapon>> dropped_weapons,
              std::string theme = "desert"):
            map_name(std::move(map_name)),
            map_width(map_width),
            map_height(map_height),
            structures(structures),
            bomb_sites(std::move(bomb_sites)),
            ct_site(std::move(ct_site)),
            tt_site(std::move(tt_site)),
            dropped_weapons(std::move(dropped_weapons)),
            theme(std::move(theme)) {}
};


#endif  // MAP_CONFIG_H
