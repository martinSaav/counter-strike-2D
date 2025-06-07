//
// Created by matias on 06/06/25.
//

#ifndef SHOOT_RESULT_H
#define SHOOT_RESULT_H
#include <memory>
#include <utility>

#include "player.h"

class ShootResult {
public:
    const bool miss;
    const int dmg;
    const std::optional<std::shared_ptr<Player>> player_hit;
    ShootResult(): miss(true), dmg(0), player_hit(std::nullopt) {}
    ShootResult(const int dmg, std::shared_ptr<Player> player_hit):
            miss(false), dmg(dmg), player_hit(std::move(player_hit)) {}
};


#endif  // SHOOT_RESULT_H
