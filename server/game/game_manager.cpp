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


bool GameManager::can_player_buy() {
    if (clock.getCurrentStage() != Stages::BuyStage) {
        return false;
    }
    return true;
}

bool GameManager::can_player_move_or_shoot(const std::shared_ptr<Player>& player) {
    if (clock.getCurrentStage() == Stages::BuyStage || player->is_dead()) {
        return false;
    }
    return true;
}


void GameManager::advance_round() {
    current_round++;
    round_won = false;
}


void GameManager::check_winning_cond(const std::vector<std::shared_ptr<Player>>& players) {
    if (round_won) {
        return;
    }
    int alive_terrorists = 0;
    int alive_counter_terrorists = 0;
    for (auto& player: players) {
        if (!player->is_dead()) {
            if (player->get_team() == Team::Terrorists) {
                alive_terrorists++;
            } else {
                alive_counter_terrorists++;
            }
        }
    }
    if (clock.has_bomb_exploded()) {
        tt_rounds++;
        last_winner = Team::Terrorists;
        round_won = true;
    } else if (bomb_defused) {
        ct_rounds++;
        last_winner = Team::CounterTerrorists;
        clock.set_after_round_stage();
        round_won = true;
    } else if (alive_counter_terrorists == 0) {
        tt_rounds++;
        last_winner = Team::Terrorists;
        clock.set_after_round_stage();
        round_won = true;
    } else if (alive_terrorists == 0 && !bomb_planted) {
        ct_rounds++;
        last_winner = Team::CounterTerrorists;
        clock.set_after_round_stage();
        round_won = true;
    }
}


TimeInformation GameManager::get_time_information() const {
    if (bomb_planted) {
        return TimeInformation{current_round, static_cast<float>(clock.get_time() / 1000),
                               !round_won,    round_won,
                               bomb_planted,  bomb_x,
                               bomb_y,        static_cast<float>(clock.get_bomb_timer()),
                               last_winner};
    }
    return TimeInformation{current_round,
                           static_cast<float>(clock.get_time() / 1000),
                           !round_won,
                           round_won,
                           bomb_planted,
                           0,
                           0,
                           0,
                           last_winner};
}
