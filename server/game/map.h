#ifndef MAP_H
#define MAP_H
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "chunk.h"
#include "player.h"
#include "structure.h"

struct StructureInPosition: public std::runtime_error {
    StructureInPosition(): std::runtime_error("There is already a structure in that position") {}
};

struct PositionOutOfRange: public std::runtime_error {
    PositionOutOfRange(): std::runtime_error("Position is out of range") {}
};

class Map {
    std::map<std::pair<int, int>, Chunk> structure_chunks;
    std::vector<std::shared_ptr<Player>> players;
    static std::pair<int, int> get_chunk_index(int x, int y);
    int max_x;
    int max_y;

public:
    Map(const int max_x, const int max_y): max_x(max_x), max_y(max_y) {}
    void add_structure(Structure structure);
    void add_player(std::shared_ptr<Player> player);
    std::vector<Structure> get_structures_near_player(const std::shared_ptr<Player>& player);
    static std::vector<std::pair<int, int>> calculate_player_chunks(int bottom_x, int bottom_y);
    std::vector<std::shared_ptr<Player>> get_near_players(
            const std::shared_ptr<Player>& player) const;
    [[nodiscard]] bool check_if_position_is_in_range(int x, int y) const;
};


#endif  // MAP_H
