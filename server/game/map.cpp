#include "map.h"

#include <cmath>
#include <set>

#include "collision_detector.h"

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
    std::set<std::pair<int, int>> chunks_indexes;
    const auto chunk_index_1 = get_chunk_index(x, y);
    const auto chunk_index_2 = get_chunk_index(x + structure.width, y);
    const auto chunk_index_3 = get_chunk_index(x, y + structure.height);
    const auto chunk_index_4 = get_chunk_index(x + structure.width, y + structure.height);
    chunks_indexes.insert(chunk_index_1);
    chunks_indexes.insert(chunk_index_2);
    chunks_indexes.insert(chunk_index_3);
    chunks_indexes.insert(chunk_index_4);
    for (const auto& chunk_idx: chunks_indexes) {
        if (!structure_chunks.contains(chunk_idx)) {
            continue;
        }
        for (Chunk& chunk = structure_chunks[chunk_idx];
             const auto& structure2: chunk.get_structures()) {
            if (CollisionDetector::check_collision_between_structures(structure, structure2)) {
                throw StructureInPosition();
            }
        }
    }
    for (const auto& chunk_idx: chunks_indexes) {
        Chunk& chunk = structure_chunks[chunk_idx];
        chunk.add_structure(structure);
    }
}


void Map::add_player(Player* player) { players.emplace_back(player); }


bool Map::check_if_position_is_in_range(const int x, const int y) const {
    return x >= 0 && x < max_x && y >= 0 && y < max_y;
}


std::vector<Structure> Map::get_structures_near_player(Player& player) {
    auto [x, y] = player.get_location();
    std::set<std::pair<int, int>> chunks_indexes;
    const auto chunk_index_1 = get_chunk_index(x, y);
    const auto chunk_index_2 = get_chunk_index(x + player_hitbox_width, y);
    const auto chunk_index_3 = get_chunk_index(x, y + player_hitbox_height);
    const auto chunk_index_4 = get_chunk_index(x + player_hitbox_width, y + player_hitbox_height);
    chunks_indexes.insert(chunk_index_1);
    chunks_indexes.insert(chunk_index_2);
    chunks_indexes.insert(chunk_index_3);
    chunks_indexes.insert(chunk_index_4);
    std::set<Structure> structures;
    for (const auto& chunk_idx: chunks_indexes) {
        if (!structure_chunks.contains(chunk_idx)) {
            continue;
        }
        for (Chunk& chunk = structure_chunks[chunk_idx];
             const auto& structure: chunk.get_structures()) {
            structures.emplace(structure);
        }
    }
    std::vector<Structure> structures_vector(structures.begin(), structures.end());
    return structures_vector;
}


std::vector<std::pair<int, int>> Map::calculate_player_chunks(int bottom_x, int bottom_y) {
    std::set<std::pair<int, int>> chunks_indexes;
    const auto chunk_index_1 = get_chunk_index(bottom_x, bottom_y);
    const auto chunk_index_2 = get_chunk_index(bottom_x + player_hitbox_width, bottom_y);
    const auto chunk_index_3 = get_chunk_index(bottom_x, bottom_y + player_hitbox_height);
    const auto chunk_index_4 =
            get_chunk_index(bottom_x + player_hitbox_width, bottom_y + player_hitbox_height);
    chunks_indexes.insert(chunk_index_1);
    chunks_indexes.insert(chunk_index_2);
    chunks_indexes.insert(chunk_index_3);
    chunks_indexes.insert(chunk_index_4);
    std::vector chunks_indexes_vector(chunks_indexes.begin(), chunks_indexes.end());
    return chunks_indexes_vector;
}


std::vector<Player*> Map::get_near_players(Player& player) {
    std::vector<Player*> near_players;
    for (auto other_player: players) {
        if (*other_player == player) {
            continue;
        }
        std::vector<std::pair<int, int>>& other_player_chunks = (*other_player).get_chunk_idxs();
        std::vector<std::pair<int, int>>& player_chunks = player.get_chunk_idxs();
        std::vector<std::pair<int, int>> intersection;
        std::set_intersection(other_player_chunks.begin(), other_player_chunks.end(),
                              player_chunks.begin(), player_chunks.end(),
                              std::back_inserter(intersection));
        if (!intersection.empty()) {
            near_players.emplace_back(other_player);
        }
    }
    return near_players;
}
