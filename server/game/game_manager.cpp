//
// Created by matias on 06/06/25.
//

#include "game_manager.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "map.h"

void GameManager::attack_player(const std::shared_ptr<Player>& attacked, Player& attacker,
                                const int damage) const {
    if (attacked->current_team == attacker.current_team || attacked->is_dead()) {
        return;
    }
    attacked->receive_damage(*this, damage);
    if (attacked->is_dead()) {
        attacker.add_kill();
    }
}


bool GameManager::can_player_buy() const {
    if (clock.getCurrentStage() != Stages::BuyStage) {
        return false;
    }
    return true;
}

bool GameManager::can_player_move_or_shoot(const std::shared_ptr<Player>& player) const {
    if (clock.getCurrentStage() == Stages::BuyStage || player->is_dead()) {
        return false;
    }
    return true;
}


void GameManager::advance_round(const std::vector<std::shared_ptr<Player>>& players) {
    for (const auto& player: players) {
        if (player->current_team == last_winner) {
            if (current_round != 6) {
                player->money += round_winner_money;
            } else {
                player->money += round_loser_money;
            }

        } else {
            if (current_round != 6) {
                player->money += round_loser_money;
            } else {
                player->money += round_winner_money;
            }
        }
    }
    current_round++;
    round_won = false;
    bomb_defused = false;
    bomb_planted = false;
    has_exploded_bomb = false;
    bomb_x = 0;
    bomb_y = 0;
}


void GameManager::check_winning_cond(const std::vector<std::shared_ptr<Player>>& players) {
    if (round_won) {
        return;
    }
    int alive_terrorists = 0;
    int alive_counter_terrorists = 0;
    for (const auto& player: players) {
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
        explode_bomb(players);
    } else if (bomb_defused || (alive_terrorists == 0 && !bomb_planted)) {
        ct_rounds++;
        last_winner = Team::CounterTerrorists;
        if (bomb_defused) {
            clock.defuse_bomb();
        } else {
            clock.set_after_round_stage();
        }
        round_won = true;
    } else if (alive_counter_terrorists == 0) {
        tt_rounds++;
        last_winner = Team::Terrorists;
        clock.set_after_round_stage();
        round_won = true;
    }
}


TimeInformation GameManager::get_time_information() const {
    if (bomb_planted) {
        return TimeInformation{current_round, static_cast<float>(clock.get_time() / 1000),
                               !round_won,    round_won,
                               bomb_planted,  bomb_x,
                               bomb_y,        static_cast<float>(clock.get_bomb_timer() / 1000),
                               last_winner};
    }
    return TimeInformation{current_round,
                           static_cast<float>(clock.get_time() / 1000),
                           !round_won,
                           round_won,
                           bomb_planted,
                           0,
                           0,
                           static_cast<float>(clock.get_bomb_timer() / 1000),
                           last_winner};
}


void GameManager::plant_bomb(const int x, const int y) {
    map.plant_bomb(x, y);
    clock.set_post_plant_stage();
    bomb_planted = true;
    bomb_x = x;
    bomb_y = y;
}


void GameManager::explode_bomb(const std::vector<std::shared_ptr<Player>>& players) {
    has_exploded_bomb = true;
    for (auto& player: players) {
        if (!player->is_dead()) {
            auto [p_x, p_y] = player->get_location();
            const double distance_to_bomb = sqrt(pow(p_x - bomb_x, 2) + pow(p_y - bomb_y, 2));
            const double damage = bomb_dmg / (1 + distance_to_bomb / 10);
            player->receive_damage(*this, static_cast<int>(damage));
        }
    }
}


bool GameManager::can_plant_bomb(const int x, const int y) const {
    return map.can_plant_bomb(x, y);
}


void GameManager::start_defusing(const std::shared_ptr<Player>& player) const {
    if (player->current_team != Team::CounterTerrorists) {
        return;
    }
    if (!map.can_defuse(player)) {
        return;
    }
    player->start_defusing();
    player->defuse_time = clock.get_time();
}


void GameManager::has_finished_defusing(Player& player) {
    if (!player.is_defusing || player.is_dead()) {
        return;
    }
    if (clock.get_time() - player.defuse_time - time_to_defuse >= 0) {
        player.is_defusing = false;
        bomb_defused = true;
        clock.defuse_bomb();
    }
}


void GameManager::drop_bomb(Player& player, std::unique_ptr<BombEncapsulator> bomb) const {
    auto [x, y] = player.get_location();
    map.drop_bomb(std::move(bomb), x, y);
}


void GameManager::drop_weapon(Player& player, std::unique_ptr<Gun> gun) const {
    auto [x, y] = player.get_location();
    map.drop_weapon(std::move(gun), x, y);
}


void GameManager::pick_weapon(const std::shared_ptr<Player>& player) const {
    if (player->is_dead()) {
        return;
    }
    auto [x, y] = player->get_location();
    if (player->current_team != Team::CounterTerrorists) {
        if (std::unique_ptr<BombEncapsulator> bomb = map.pick_bomb(x, y); bomb != nullptr) {
            player->equip_bomb(std::move(bomb));
            return;
        }
    }
    if (std::unique_ptr<Gun> gun = map.pick_weapon(x, y); gun != nullptr) {
        player->equip_weapon(std::move(gun));
    }
}


bool GameManager::has_to_switch_sides() const { return current_round == 6; }


bool GameManager::has_ended() const {
    if (tt_rounds >= 6) {
        return true;
    }
    if (ct_rounds >= 6) {
        return true;
    }
    if (current_round > number_of_rounds) {
        return true;
    }
    return false;
}


void GameManager::give_bomb_to_random_player(
        const std::vector<std::shared_ptr<Player>>& players) const {
    if (tt_count == 0) {
        return;
    }
    const int bomb_player = std::rand() % (tt_count);
    int current_tt = 0;
    for (auto& player: players) {
        if (player->current_team == Team::Terrorists) {
            if (current_tt == bomb_player) {
                player->equip_bomb(std::make_unique<BombEncapsulator>(config));
                return;
            }
            current_tt++;
        }
    }
}


Team GameManager::get_match_winner() const {
    if (tt_rounds > ct_rounds) {
        return Team::Terrorists;
    }
    if (ct_rounds > tt_rounds) {
        return Team::CounterTerrorists;
    }
    return Team::Terrorists;
}


void GameManager::switch_sides() {
    const int temp = ct_rounds;
    ct_rounds = tt_rounds;
    tt_rounds = temp;
    const int temp_amount = ct_amount;
    ct_amount = tt_amount;
    tt_amount = temp_amount;
    const int temp_count = ct_count;
    ct_count = tt_count;
    tt_count = temp_count;
}


void GameManager::set_players_spawn(const std::vector<std::shared_ptr<Player>>& players) const {
    std::vector<std::shared_ptr<Player>> terrorists;
    std::vector<std::shared_ptr<Player>> counter_terrorists;
    for (auto& player: players) {
        if (player->current_team == Team::Terrorists) {
            terrorists.push_back(player);
        } else {
            counter_terrorists.push_back(player);
        }
    }
    const Site& ct_site = map.get_ct_site();
    const Site& tt_site = map.get_tt_site();
    ct_site.assign_spawns_to_players(map, counter_terrorists);
    tt_site.assign_spawns_to_players(map, terrorists);
}


Team GameManager::get_next_player_team() const {
    if (ct_amount == ct_count && tt_amount == tt_count) {
        throw TeamsAreFull();
    }
    if (ct_count == ct_amount) {
        return Team::Terrorists;
    }
    if (ct_count < tt_count) {
        return Team::CounterTerrorists;
    }
    return Team::Terrorists;
}


void GameManager::add_player_to_team(const std::shared_ptr<Player>& player) {
    if (player->current_team == Team::Terrorists) {
        tt_count++;
    } else {
        ct_count++;
    }
}


void GameManager::remove_player_from_team(const std::shared_ptr<Player>& player) {
    if (player->current_team == Team::Terrorists) {
        tt_count--;
    } else {
        ct_count--;
    }
}


bool GameManager::exploded_bomb() const { return has_exploded_bomb; }
