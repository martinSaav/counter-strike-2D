#ifndef GAMESTATEUPDATER_H
#define GAMESTATEUPDATER_H
#include <memory>
#include <vector>

#include "gun/gun.h"

#include "map.h"


class GameStateUpdater {
    Map& map;
    std::vector<std::shared_ptr<Gun>> guns;

public:
    explicit GameStateUpdater(Map& map);
    void update_game(float time_step);
};


#endif  // GAMESTATEUPDATER_H
