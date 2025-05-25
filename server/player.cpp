#include "player.h"

std::pair<int, int> Player::get_location() { return {position_x, position_y}; }

void Player::set_location(const Position position) {
    auto [new_x, new_y] = position.get_position();
    position_x = new_x;
    position_y = new_y;
}

PlayerDTO Player::get_player_info() const { return PlayerDTO{username, position_x, position_y}; }
