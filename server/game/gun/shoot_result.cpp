//
// Created by matias on 07/06/25.
//
#include "shoot_result.h"

#include <utility>

#include "player.h"

ShootResult::ShootResult(const std::pair<int, int>& position):
        miss(true), dmg(0), position(position), player_hit(std::nullopt) {}
ShootResult::ShootResult(int dmg, const std::pair<int, int>& position,
                         std::shared_ptr<Player> player_hit):
        miss(false), dmg(dmg), position(position), player_hit(std::move(player_hit)) {}
