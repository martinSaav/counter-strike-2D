#ifndef POSITION_H
#define POSITION_H
#include <stdexcept>
#include <utility>

struct InvalidPosition: public std::runtime_error {
    InvalidPosition(): std::runtime_error("Invalid position") {}
};


class Position {
    int x;
    int y;

public:
    Position(int x, int y);
    [[nodiscard]] std::pair<int, int> get_position() const;
};


#endif  // POSITION_H
