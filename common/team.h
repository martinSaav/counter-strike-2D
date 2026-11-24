#ifndef TEAM_H
#define TEAM_H

#include <cstdint>

enum class Team : uint8_t {
    None = 0x00,  // sin ganador: la partida termino empatada
    Terrorists = 0x01,
    CounterTerrorists = 0x02,
};

#endif
