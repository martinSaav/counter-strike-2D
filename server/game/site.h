//
// Created by matias on 19/06/25.
//

#ifndef SITE_H
#define SITE_H

#include <memory>
#include <utility>
#include <vector>

class Player;
class Map;

class Site {
public:
    const int x;
    const int y;
    const int site_width;
    const int site_height;
    const std::vector<std::pair<int, int>> spawns;
    Site(int x, int y, int width, int height, std::vector<std::pair<int, int>>&& spawns);
    void assign_spawns_to_players(const Map& map,
                                  const std::vector<std::shared_ptr<Player>>& players) const;
};


#endif  // SITE_H
