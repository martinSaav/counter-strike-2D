//
// Created by matias on 24/05/25.
//

#include "structure.h"

Structure::Structure(const int height, const int width, const Position position):
        height(height), width(width), position(position) {}


bool Structure::check_if_inside(const int x, const int y) const {
    if (auto [pos_x, pos_y] = position.get_position();
        (x >= pos_x && x <= pos_x + width) && (y >= pos_y && y <= pos_y + height)) {
        return true;
    }
    return false;
}

std::pair<int, int> Structure::get_position() const { return position.get_position(); }
