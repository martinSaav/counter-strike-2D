#ifndef PLAYERDTO_H
#define PLAYERDTO_H
#include <string>
#include <utility>

#include "dto/player_info.h"

#include "player_skin.h"

class PlayerDTO {
public:
    const std::string username;
    const int position_x;
    const int position_y;
    const PlayerSkin skin;
    const int health;
    const Status status;
    const int money;
    int kills;
    int deaths;
    PlayerDTO(std::string username, const int position_x, const int position_y,
              const PlayerSkin skin, const int health, const Status status, const int money,
              const int kills, const int deaths):
            username(std::move(username)),
            position_x(position_x),
            position_y(position_y),
            skin(skin),
            health(health),
            status(status),
            money(money),
            kills(kills),
            deaths(deaths) {}
};
#endif  // PLAYERDTO_H
