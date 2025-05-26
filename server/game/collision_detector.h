#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H
#include <utility>

#include "player.h"
#include "structure.h"

class CollisionDetector {
    static bool check_collision_between_objects(const std::pair<int, int>& object1_bottom_coords,
                                                const std::pair<int, int>& object1_top_coords,
                                                const std::pair<int, int>& object2_bottom_coords,
                                                const std::pair<int, int>& object2_top_coords);

public:
    static bool check_collision_between_player_and_structure(Player& player,
                                                             const Structure& structure);

    static bool check_collision_between_structures(const Structure& structure1,
                                                   const Structure& structure2);

    static bool check_collision_between_players(Player& player1, Player& player2);
};


#endif  // COLLISION_CHECKER_H
