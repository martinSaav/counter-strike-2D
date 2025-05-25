#include "position.h"

Position::Position(const int x, const int y): x(x), y(y) {
    if ((x < 0) || (y < 0)) {
        throw InvalidPosition();
    }
}

std::pair<int, int> Position::get_position() const { return std::make_pair(x, y); }
