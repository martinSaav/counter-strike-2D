//
// Created by matias on 24/05/25.
//

#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <utility>

#include "position.h"

class Structure {
    const Position position;

public:
    const int height;
    const int width;

    Structure(int height, int width, Position position);
    [[nodiscard]] bool check_if_inside(int x, int y) const;
    [[nodiscard]] std::pair<int, int> get_position() const;
};


#endif  // STRUCTURE_H
