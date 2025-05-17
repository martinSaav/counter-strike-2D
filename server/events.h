//
// Created by matias on 15/05/25.
//

#ifndef EVENTS_H
#define EVENTS_H
#include <cstdint>

enum class CommandTypes : uint8_t {
    listMatches,
    createMatch,
    joinMatch,
    moveUp,
    moveDown,
    moveLeft,
    moveRight,
    invalid
};

#endif  // EVENTS_H
