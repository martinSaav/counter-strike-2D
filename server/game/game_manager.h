//
// Created by matias on 06/06/25.
//

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#include <memory>

#include "player.h"


class GameManager {
public:
    GameManager();
    void attack_player(std::shared_ptr<Player> attacked, std::shared_ptr<Player> attacker,
                       int damage);
};


#endif  // GAME_MANAGER_H
