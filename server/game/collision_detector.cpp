#include "collision_detector.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>

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


namespace {

// Interseccion del segmento ini -> fin con el rectangulo [min, max] (bordes incluidos), por el
// metodo de las "slabs". Devuelve el parametro t en [0, 1] del punto de entrada, o nullopt si no
// se tocan. El calculo anterior dividia por la componente de la velocidad y daba NaN con los
// disparos exactamente verticales u horizontales.
std::optional<double> segment_entry(const int ini_x, const int ini_y, const int fin_x,
                                    const int fin_y, const int min_x, const int min_y,
                                    const int max_x, const int max_y) {
    const double origin[2] = {static_cast<double>(ini_x), static_cast<double>(ini_y)};
    const double delta[2] = {static_cast<double>(fin_x - ini_x),
                             static_cast<double>(fin_y - ini_y)};
    const double low[2] = {static_cast<double>(min_x), static_cast<double>(min_y)};
    const double high[2] = {static_cast<double>(max_x), static_cast<double>(max_y)};
    double t_enter = 0.0;
    double t_exit = 1.0;
    for (int axis = 0; axis < 2; axis++) {
        if (delta[axis] == 0) {
            // El segmento es paralelo a este eje: o esta dentro de la franja o no la toca nunca
            if (origin[axis] < low[axis] || origin[axis] > high[axis]) {
                return std::nullopt;
            }
            continue;
        }
        double t_1 = (low[axis] - origin[axis]) / delta[axis];
        double t_2 = (high[axis] - origin[axis]) / delta[axis];
        if (t_1 > t_2) {
            std::swap(t_1, t_2);
        }
        t_enter = std::max(t_enter, t_1);
        t_exit = std::min(t_exit, t_2);
        if (t_enter > t_exit) {
            return std::nullopt;
        }
    }
    return t_enter;
}


std::optional<std::pair<int, int>> segment_impact(const int ini_x, const int ini_y, const int fin_x,
                                                  const int fin_y, const int min_x, const int min_y,
                                                  const int max_x, const int max_y) {
    const std::optional<double> t =
            segment_entry(ini_x, ini_y, fin_x, fin_y, min_x, min_y, max_x, max_y);
    if (!t.has_value()) {
        return std::nullopt;
    }
    const double impact_x = ini_x + t.value() * (fin_x - ini_x);
    const double impact_y = ini_y + t.value() * (fin_y - ini_y);
    return std::make_pair(static_cast<int>(std::lround(impact_x)),
                          static_cast<int>(std::lround(impact_y)));
}

}  // namespace


std::optional<std::pair<int, int>> CollisionDetector::get_bullet_impact(const Structure& structure,
                                                                        const int ini_x,
                                                                        const int ini_y,
                                                                        const int final_x,
                                                                        const int final_y) {
    auto [s_bottom_x, s_bottom_y] = structure.get_position();
    return segment_impact(ini_x, ini_y, final_x, final_y, s_bottom_x, s_bottom_y,
                          s_bottom_x + structure.width, s_bottom_y + structure.height);
}


std::optional<std::pair<int, int>> CollisionDetector::get_bullet_impact(
        const int p_bottom_x, const int p_bottom_y, const int ini_x, const int ini_y,
        const int final_x, const int final_y) {
    return segment_impact(ini_x, ini_y, final_x, final_y, p_bottom_x, p_bottom_y,
                          p_bottom_x + player_hitbox_width, p_bottom_y + player_hitbox_height);
}
