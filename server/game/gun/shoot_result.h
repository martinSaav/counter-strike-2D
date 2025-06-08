//
// Created by matias on 06/06/25.
//

#ifndef SHOOT_RESULT_H
#define SHOOT_RESULT_H
#include <memory>
#include <optional>
#include <utility>

class Player;

class ShootResult {
public:
    const bool miss;
    const int dmg;
    const std::pair<int, int> position;
    const std::optional<std::shared_ptr<Player>> player_hit;

    explicit ShootResult(const std::pair<int, int>& position);
    ShootResult(int dmg, const std::pair<int, int>& position, std::shared_ptr<Player> player_hit);
};


#endif  // SHOOT_RESULT_H
