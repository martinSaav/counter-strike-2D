#ifndef GAMESTATEUPDATER_H
#define GAMESTATEUPDATER_H
#include <chrono>
#include <memory>
#include <vector>

#include "gun/gun.h"

#include "map.h"


class GameStateUpdater {
    Map& map;
    std::vector<std::shared_ptr<Gun>> guns;

public:
    explicit GameStateUpdater(Map& map);
    void update_game(std::chrono::time_point<std::chrono::system_clock> current_time);
};


#endif  // GAMESTATEUPDATER_H
