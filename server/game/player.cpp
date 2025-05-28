#include "player.h"

#include <utility>


std::pair<int, int> Player::get_location() { return std::make_pair(position_x, position_y); }


void Player::set_location(const Position position, std::vector<std::pair<int, int>>&& chunks_idxs) {
    auto [new_x, new_y] = position.get_position();
    position_x = new_x;
    position_y = new_y;

    this->chunks_idxs = std::move(chunks_idxs);
}


PlayerDTO Player::get_player_info() const { return PlayerDTO{username, position_x, position_y}; }
