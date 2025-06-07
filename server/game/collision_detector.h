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
    static bool check_collision_between_player_and_structure(int p_bottom_x, int p_bottom_y,
                                                             const Structure& structure);

    static std::pair<int, int> get_collision_point_between_bullet_and_object(
            double inter_min_y, double inter_max_y, int ini_x, int ini_y,
            std::pair<double, double> velocity);

    static std::pair<int, int> get_impact_point(const Structure& structure, int ini_x, int ini_y,
                                                int final_x,
                                                const std::pair<double, double>& velocity);

    static std::pair<int, int> get_impact_point(int p_bottom_x, int p_bottom_y, int ini_x,
                                                int ini_y, int final_x,
                                                const std::pair<double, double>& velocity);

    static bool check_collision_between_structures(const Structure& structure1,
                                                   const Structure& structure2);

    static bool check_collision_between_players(int p1_bottom_x, int p1_bottom_y, Player& player2);

    static bool check_collision_between_structure_and_bullet(
            const Structure& structure, int ini_x, int ini_y, int final_x,
            const std::pair<double, double>& velocity);

    static bool check_collision_between_player_and_bullet(
            int p_bottom_x, int p_bottom_y, int ini_x, int ini_y, int final_x,
            const std::pair<double, double>& velocity);
};


#endif  // COLLISION_CHECKER_H
