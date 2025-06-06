//
// Created by matias on 06/06/25.
//

#include "game_manager.h"


void GameManager::attack_player(const std::shared_ptr<Player>& attacked, Player& attacker,
                                const int damage) {
    if (attacked->current_team == attacker.current_team) {
        return;
    }
    attacked->receive_damage(damage);
    if (attacked->is_dead()) {
        attacker.add_kill();
    }
}
