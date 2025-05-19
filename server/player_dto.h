#ifndef PLAYERDTO_H
#define PLAYERDTO_H
#include <string>

class PlayerDTO {
public:
    const std::string username;
    const int position_x;
    const int position_y;
    PlayerDTO(const std::string& username, const int position_x, const int position_y):
            username(username), position_x(position_x), position_y(position_y) {}
};

#endif  // PLAYERDTO_H
