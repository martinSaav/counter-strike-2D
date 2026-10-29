#ifndef CHUNK_H
#define CHUNK_H

// Lado (en tiles) de cada chunk en el que se divide el mapa para buscar colisiones solo cerca
constexpr int CHUNK_SIZE = 100;
#include <vector>

#include "structure.h"

class Chunk {
    std::vector<Structure> structures;

public:
    void add_structure(Structure structure);
    [[nodiscard]] const std::vector<Structure>& get_structures() const;
};


#endif  // CHUNK_H
