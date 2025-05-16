//
// Created by matias on 15/05/25.
//

#include "player.h"

std::pair<int, int> Player::get_location() { return {position_x, position_y}; }

void Player::set_location(const int x, const int y) {
    position_x = x;
    position_y = y;
}

PlayerDTO Player::get_player_info() const { return PlayerDTO{username, position_x, position_y}; }
