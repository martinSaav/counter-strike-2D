#include "map.h"

#include <cmath>

std::pair<int, int> Map::get_chunk_index(int x, int y) {
    int idx_x = std::floor(x / CHUNK_SIZE);
    int idx_y = std::floor(y / CHUNK_SIZE);
    return {idx_x, idx_y};
}


void Map::add_structure(const Structure structure) {
    auto [x, y] = structure.get_position();
    if (!check_if_position_is_in_range(x, y)) {
        throw PositionOutOfRange();
    }
    if (check_if_there_is_a_structure_in_pos(x, y) ||
        check_if_there_is_a_structure_in_pos(x + structure.width, y) ||
        check_if_there_is_a_structure_in_pos(x, y + structure.height) ||
        check_if_there_is_a_structure_in_pos(x + structure.width, y + structure.height)) {
        throw StructureInPosition();
    }
    const auto chunk_index_1 = get_chunk_index(
            x, y);  // índice del chunk del vertice superior izquierdo de la estructura
    const auto chunk_index_2 = get_chunk_index(
            x + structure.width, y);  // índice del chunk del vertice derecho de la estructura
    const auto chunk_index_3 =
            get_chunk_index(x, y + structure.height);  // índice del chunk del vertice inferior
                                                       // izquierdo de la estructura
    const auto chunk_index_4 = get_chunk_index(
            x + structure.height, y + structure.height);  // índice del chunk del vertice inferior
                                                          // derecho de la estructura
    Chunk& chunk_1 = structure_chunks[chunk_index_1];
    Chunk& chunk_2 = structure_chunks[chunk_index_2];
    Chunk& chunk_3 = structure_chunks[chunk_index_3];
    Chunk& chunk_4 = structure_chunks[chunk_index_4];
    chunk_1.add_structure(structure);
    chunk_2.add_structure(structure);
    chunk_3.add_structure(structure);
    chunk_4.add_structure(structure);
}

bool Map::check_if_there_is_a_structure_in_pos(const int x, const int y) const {
    auto chunk_idx = get_chunk_index(x, y);
    auto structures_p = structure_chunks.find(chunk_idx);
    if (structures_p == structure_chunks.end()) {
        return false;
    }

    const std::vector<Structure>& structures = structures_p->second.get_structures();
    return std::any_of(structures.begin(), structures.end(), [x, y](const Structure& structure) {
        return structure.check_if_inside(x, y);
    });
}


bool Map::check_if_position_is_in_range(const int x, const int y) const {
    return x >= 0 && x < max_x && y >= 0 && y < max_y;
}
