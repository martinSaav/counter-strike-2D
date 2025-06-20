//
// Created by matias on 19/06/25.
//

#include "site.h"

#include <chrono>
#include <random>

#include "map.h"
#include "player.h"

Site::Site(const int x, const int y, const int width, const int height,
           std::vector<std::pair<int, int> >&& spawns):
        x(x), y(y), site_width(width), site_height(height), spawns(std::move(spawns)) {}


void Site::assign_spawns_to_players(const Map& map,
                                    const std::vector<std::shared_ptr<Player> >& players) const {
    auto shuffled_spawns = spawns;
    const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(shuffled_spawns.begin(), shuffled_spawns.end(), rng);
    for (size_t i = 0; i < players.size() && i < shuffled_spawns.size(); ++i) {
        auto [x, y] = shuffled_spawns[i];
        const Position new_pos(x, y);
        players[i]->set_location(new_pos, map.calculate_player_chunks(x, y));
    }
}
