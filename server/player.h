#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <utility>

#include "player_dto.h"
#include "position.h"
#define player_hitbox_height 5
#define player_hitbox_width 5

class Player {
    const std::string username;
    int position_x;
    int position_y;

public:
    Player(const std::string& username, const int position_x, const int position_y):
            username(username), position_x(position_x), position_y(position_y) {}
    std::pair<int, int> get_location();
    void set_location(Position position);
    [[nodiscard]] PlayerDTO get_player_info() const;
};

#endif  // PLAYER_H
