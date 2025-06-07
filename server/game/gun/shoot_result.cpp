//
// Created by matias on 07/06/25.
//
#include "shoot_result.h"

#include <utility>

#include "player.h"

ShootResult::ShootResult(): miss(true), dmg(0), player_hit(std::nullopt) {}
ShootResult::ShootResult(const int dmg, std::shared_ptr<Player> player_hit):
        miss(false), dmg(dmg), player_hit(std::move(player_hit)) {}
