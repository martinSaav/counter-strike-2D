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


    bool operator<(const Structure& other) const {
        auto [this_x, this_y] = position.get_position();
        auto [other_x, other_y] = other.position.get_position();
        if (this_x != other_x)
            return this_x < other_x;

        if (this_y != other_y)
            return this_y < other_y;

        if (height != other.height)
            return height < other.height;

        return width < other.width;
    }
};


#endif  // STRUCTURE_H
