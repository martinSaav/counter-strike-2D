//
// Created by matias on 06/06/25.
//

#ifndef SHOOT_RESULT_H
#define SHOOT_RESULT_H
#include <memory>
#include <optional>

class Player;

class ShootResult {
public:
    const bool miss;
    const int dmg;
    const std::optional<std::shared_ptr<Player>> player_hit;
    ShootResult();
    ShootResult(int dmg, std::shared_ptr<Player> player_hit);
};


#endif  // SHOOT_RESULT_H
