#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <string>

struct Game {
    std::string name;
    std::string map_name;
    uint8_t current_players;
    uint8_t max_players;
};

#endif
