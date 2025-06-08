#ifndef MAP_H
#define MAP_H
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "chunk.h"
#include "impact_info.h"
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
    int max_x;
    int max_y;

    static std::pair<int, int> get_chunk_index(int x, int y);

    static std::pair<double, double> calculate_bullet_velocity(std::pair<int, int> starting_pos,
                                                               std::pair<int, int> ending_pos);

    static std::pair<std::pair<int, int>, std::pair<double, double>> calculate_new_bullet_position(
            const std::pair<double, double>& starting_pos,
            const std::pair<double, double>& velocity);
    [[nodiscard]] std::vector<std::shared_ptr<Player>> get_players_near_point(int x, int y) const;
    ImpactInfo get_nearest_colliding_structure(const Chunk& chunk, int ini_x, int ini_y,
                                               const Position& final_pos,
                                               const std::pair<double, double>& velocity);
    ImpactInfo get_nearest_colliding_player(int ini_x, int ini_y, double current_x,
                                            double current_y, const Player& gun_owner,
                                            const Position& final_pos,
                                            std::pair<double, double> velocity);


public:
    Map(const int max_x, const int max_y): max_x(max_x), max_y(max_y) {}
    void add_structure(Structure structure);
    void add_player(const std::shared_ptr<Player>& player);
    std::vector<Structure> get_structures_near_player(const std::shared_ptr<Player>& player);
    static std::vector<std::pair<int, int>> calculate_player_chunks(int bottom_x, int bottom_y);
    [[nodiscard]] std::vector<std::shared_ptr<Player>> get_near_players(
            const std::shared_ptr<Player>& player) const;
    [[nodiscard]] bool check_if_position_is_in_range(int x, int y) const;
    ImpactInfo trace_bullet_path(int ini_x, int ini_y, Position final_pos, const Player& gun_owner);
    std::vector<std::shared_ptr<Player>> get_players_near_radio(double x, double y,
                                                                double radio) const;
};


#endif  // MAP_H
