#ifndef PLAYERDTO_H
#define PLAYERDTO_H
#include <string>
#include <utility>

#include "player_skin.h"

class PlayerDTO {
public:
    const std::string username;
    const PlayerSkin skin;
    const int angle;
    const int position_x;
    const int position_y;
    PlayerDTO(std::string username, const int position_x, const int position_y,
              const PlayerSkin skin, const int angle):
            username(std::move(username)),
            skin(skin),
            angle(angle),
            position_x(position_x),
            position_y(position_y) {}
};

#endif  // PLAYERDTO_H
