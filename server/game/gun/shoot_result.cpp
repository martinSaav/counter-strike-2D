//
// Created by matias on 07/06/25.
//
#include "shoot_result.h"

#include <utility>

#include "player.h"

ShootResult::ShootResult(): type(ShootType::PlantingBomb), miss(false), dmg(0) {}

ShootResult::ShootResult(const std::pair<int, int>& position):
        type(ShootType::Miss), miss(true), dmg(0), position(position), player_hit(std::nullopt) {}
ShootResult::ShootResult(int dmg, const std::pair<int, int>& position,
                         std::shared_ptr<Player> player_hit):
        type(ShootType::Hit),
        miss(false),
        dmg(dmg),
        position(position),
        player_hit(std::move(player_hit)) {}
