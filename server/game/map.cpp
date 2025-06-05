#include "map.h"

#include <algorithm>
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
        for (const Chunk& chunk = structure_chunks[chunk_idx];
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


void Map::add_player(std::shared_ptr<Player> player) { players.emplace_back(player); }


bool Map::check_if_position_is_in_range(const int x, const int y) const {
    return x >= 0 && x < max_x && y >= 0 && y < max_y;
}


std::vector<Structure> Map::get_structures_near_player(const std::shared_ptr<Player>& player) {
    auto [x, y] = player->get_location();
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
        for (const Chunk& chunk = structure_chunks[chunk_idx];
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


std::vector<std::shared_ptr<Player>> Map::get_near_players(
        const std::shared_ptr<Player>& player) const {
    std::vector<std::shared_ptr<Player>> near_players;
    for (const auto& other_player: players) {
        if (other_player == player) {
            continue;
        }
        std::vector<std::pair<int, int>>& other_player_chunks = other_player->get_chunk_idxs();
        std::vector<std::pair<int, int>>& player_chunks = player->get_chunk_idxs();
        std::vector<std::pair<int, int>> intersection;
        std::ranges::set_intersection(other_player_chunks, player_chunks,
                                      std::back_inserter(intersection));
        if (!intersection.empty()) {
            near_players.emplace_back(other_player);
        }
    }
    return near_players;
}


std::optional<std::variant<Structure, std::shared_ptr<Player>>> Map::trace_bullet_path(
        int ini_x, int ini_y, Position final_pos) {
    std::pair<int, int> final_pos_v = final_pos.get_position();
    double cos =
            (ini_x * final_pos_v.first + ini_y * final_pos_v.second) /
            (sqrt(ini_x * ini_y + ini_y * ini_x) *
             sqrt(final_pos_v.first * final_pos_v.first + final_pos_v.second * final_pos_v.second));
    double angle = acos(cos);
    while (angle < 0) {
        angle += 2 * M_PI;
    }
    angle = fmod(angle, 2 * M_PI);
    while (true) {
        std::pair<int, int> chunk_idx = get_chunk_index(ini_x, ini_y);
        auto chunk_p = structure_chunks.find(chunk_idx);
        if (chunk_p == structure_chunks.end()) {
            if (std::make_pair<int, int>{ini_x, ini_y} == final_pos.get_position()) {
                return std::nullopt;
            }
        } else {
            Chunk chunk = chunk_p->second;
            for (const auto& structure: chunk.get_structures()) {}
        }
        ini_x = new_pos.first;
        ini_y = new_pos.second;
    }
}
