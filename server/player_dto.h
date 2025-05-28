#ifndef PLAYERDTO_H
#define PLAYERDTO_H
#include <string>
#include <utility>
class PlayerDTO {
public:
    const std::string username;
    const int position_x;
    const int position_y;
    PlayerDTO(std::string username, const int position_x, const int position_y):
            username(std::move(username)), position_x(position_x), position_y(position_y) {}
};

#endif  // PLAYERDTO_H
