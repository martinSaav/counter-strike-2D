#include "collision_detector.h"

#include <algorithm>

bool CollisionDetector::check_collision_between_objects(
        const std::pair<int, int>& object1_bottom_coords,
        const std::pair<int, int>& object1_top_coords,
        const std::pair<int, int>& object2_bottom_coords,
        const std::pair<int, int>& object2_top_coords) {
    if (object1_bottom_coords.first > object2_top_coords.first) {
        return false;
    }
    if (object1_top_coords.first < object2_bottom_coords.first) {
        return false;
    }
    if (object1_bottom_coords.second > object2_top_coords.second) {
        return false;
    }
    if (object1_top_coords.second < object2_bottom_coords.second) {
        return false;
    }
    return true;
}


bool CollisionDetector::check_collision_between_player_and_structure(int p_bottom_x, int p_bottom_y,
                                                                     const Structure& structure) {
    const int p_top_x = p_bottom_x + player_hitbox_width;
    const int p_top_y = p_bottom_y + player_hitbox_height;
    auto [s_bottom_x, s_bottom_y] = structure.get_position();
    const int top_s_x = s_bottom_x + structure.width;
    const int top_s_y = s_bottom_y + structure.height;
    return check_collision_between_objects({p_bottom_x, p_bottom_y}, {p_top_x, p_top_y},
                                           {s_bottom_x, s_bottom_y}, {top_s_x, top_s_y});
}

bool CollisionDetector::check_collision_between_structures(const Structure& structure1,
                                                           const Structure& structure2) {
    auto [s1_bottom_x, s1_bottom_y] = structure1.get_position();
    auto [s2_bottom_x, s2_bottom_y] = structure2.get_position();
    int s1_top_x = s1_bottom_x + structure1.width;
    int s1_top_y = s1_bottom_y + structure1.height;
    int s2_top_x = s2_bottom_x + structure2.width;
    int s2_top_y = s2_bottom_y + structure2.height;
    return check_collision_between_objects({s1_bottom_x, s1_bottom_y}, {s1_top_x, s1_top_y},
                                           {s2_bottom_x, s2_bottom_y}, {s2_top_x, s2_top_y});
}


bool CollisionDetector::check_collision_between_players(int p1_bottom_x, int p1_bottom_y,
                                                        Player& player2) {
    const int p1_top_x = p1_bottom_x + player_hitbox_width;
    const int p1_top_y = p1_bottom_y + player_hitbox_height;
    auto [p2_bottom_x, p2_bottom_y] = player2.get_location();
    const int p2_top_x = p2_bottom_x + player_hitbox_width;
    const int p2_top_y = p2_bottom_y + player_hitbox_height;
    return check_collision_between_objects({p1_bottom_x, p1_bottom_y}, {p1_top_x, p1_top_y},
                                           {p2_bottom_x, p2_bottom_y}, {p2_top_x, p2_top_y});
}


bool CollisionDetector::check_collision_between_player_and_bullet(
        const int p_bottom_x, const int p_bottom_y, const int ini_x, const int ini_y,
        const int final_x, const std::pair<double, double>& velocity) {
    const int b_bottom_x = std::min(ini_x, final_x);
    const int b_top_x = std::max(ini_x, final_x);
    const int inter_min = std::max(p_bottom_x, b_bottom_x);
    if (const int inter_max = std::min(p_bottom_x + player_hitbox_width, b_top_x);
        inter_min <= inter_max) {
        const double step_1 = (inter_min - ini_x) / velocity.first;
        const double inter_min_bullet_y = ini_y + step_1 * velocity.second;
        const double step_2 = (inter_max - ini_x) / velocity.first;
        const double inter_max_bullet_y = ini_y + step_2 * velocity.second;
        const double bullet_min_y = std::min(inter_min_bullet_y, inter_max_bullet_y);
        const double bullet_max_y = std::max(inter_min_bullet_y, inter_max_bullet_y);
        const double inter_min_y = std::max(bullet_min_y, static_cast<double>(p_bottom_y));
        if (const double inter_max_y =
                    std::min(bullet_max_y, static_cast<double>(p_bottom_y + player_hitbox_height));
            inter_min_y <= inter_max_y) {
            return true;
        }
    }
    return false;
}


bool CollisionDetector::check_collision_between_structure_and_bullet(
        const Structure& structure, const int ini_x, const int ini_y, const int final_x,
        const std::pair<double, double>& velocity) {
    const int b_bottom_x = std::min(ini_x, final_x);
    const int b_top_x = std::max(ini_x, final_x);
    auto [s_bottom_x, s_bottom_y] = structure.get_position();
    const int inter_min = std::max(s_bottom_x, b_bottom_x);
    if (const int inter_max = std::min(s_bottom_x + structure.width, b_top_x);
        inter_min <= inter_max) {
        const double step_1 = (inter_min - ini_x) / velocity.first;
        const double inter_min_bullet_y = ini_y + step_1 * velocity.second;
        const double step_2 = (inter_max - ini_x) / velocity.first;
        const double inter_max_bullet_y = ini_y + step_2 * velocity.second;
        const double bullet_min_y = std::min(inter_min_bullet_y, inter_max_bullet_y);
        const double bullet_max_y = std::max(inter_min_bullet_y, inter_max_bullet_y);
        const double inter_min_y = std::max(bullet_min_y, static_cast<double>(s_bottom_y));
        if (const double inter_max_y =
                    std::min(bullet_max_y, static_cast<double>(s_bottom_y + structure.height));
            inter_min_y <= inter_max_y) {
            return true;
        }
    }
    return false;
}
