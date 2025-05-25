#include "chunk.h"

void Chunk::add_structure(const Structure structure) {
    if (std::find(structures.begin(), structures.end(), structure) == structures.end()) {
        structures.push_back(structure);
    }
}


const std::vector<Structure>& Chunk::get_structures() const { return structures; }
