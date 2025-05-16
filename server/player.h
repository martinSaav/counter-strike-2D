//
// Created by matias on 15/05/25.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <utility>

#include "player_dto.h"

class Player {
    const std::string username;
    int position_x;
    int position_y;

public:
    Player(const std::string& username, const int position_x, const int position_y):
            position_x(position_x), position_y(position_y), username(username) {}
    std::pair<int, int> get_location();
    void set_location(int x, int y);
    PlayerDTO get_player_info() const;
};

#endif  // PLAYER_H
