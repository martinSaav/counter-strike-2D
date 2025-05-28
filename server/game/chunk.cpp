#include "chunk.h"

void Chunk::add_structure(const Structure structure) { structures.push_back(structure); }


const std::vector<Structure>& Chunk::get_structures() const { return structures; }
