//
// Created by matias on 06/06/25.
//

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <memory>

#include "player.h"


class GameManager {
    Map& map;
    float time;

public:
    explicit GameManager(Map& map): map(map), time(0) {}
    void attack_player(const std::shared_ptr<Player>& attacked, Player& attacker, int damage);
    void advance_time(const float dt) { time += dt; }
    [[nodiscard]] float get_time() const { return time; }
    [[nodiscard]] Map& get_map() const { return map; }
};


#endif  // GAME_MANAGER_H
