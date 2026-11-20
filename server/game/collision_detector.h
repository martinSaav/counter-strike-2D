#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H
#include <optional>
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

    static bool check_collision_between_structures(const Structure& structure1,
                                                   const Structure& structure2);

    static bool check_collision_between_players(int p1_bottom_x, int p1_bottom_y, Player& player2);

    // Punto donde la bala que va de (ini_x, ini_y) a (final_x, final_y) entra al objeto, o
    // nullopt si no lo toca. Funciona tambien con disparos verticales u horizontales.
    static std::optional<std::pair<int, int>> get_bullet_impact(const Structure& structure,
                                                                int ini_x, int ini_y, int final_x,
                                                                int final_y);

    static std::optional<std::pair<int, int>> get_bullet_impact(int p_bottom_x, int p_bottom_y,
                                                                int ini_x, int ini_y, int final_x,
                                                                int final_y);
};


#endif  // COLLISION_CHECKER_H
