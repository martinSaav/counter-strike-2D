#ifndef MAP_H
#define MAP_H
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "bomb_site.h"
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
    std::vector<BombSite> bomb_sites;
    std::vector<std::pair<Position, std::unique_ptr<Gun>>> dropped_guns;
    std::pair<Position, std::unique_ptr<BombEncapsulator>> dropped_bomb;
    std::optional<std::pair<int, int>> bomb_position;
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
    Map(const int x, const int y): dropped_bomb({Position(0, 0), nullptr}), max_x(x), max_y(y) {}
    void add_structure(Structure structure);
    void add_bombsite(BombSite site);
    void add_player(const std::shared_ptr<Player>& player);
    std::vector<Structure> get_structures_near_player(const std::shared_ptr<Player>& player);
    static std::vector<std::pair<int, int>> calculate_player_chunks(int bottom_x, int bottom_y);
    [[nodiscard]] std::vector<std::shared_ptr<Player>> get_near_players(
            const std::shared_ptr<Player>& player) const;
    [[nodiscard]] bool check_if_position_is_in_range(int x, int y) const;
    ImpactInfo trace_bullet_path(int ini_x, int ini_y, Position final_pos, const Player& gun_owner);
    [[nodiscard]] std::vector<std::shared_ptr<Player>> get_players_near_radio(double x, double y,
                                                                              double radio) const;
    void plant_bomb(int x, int y);
    [[nodiscard]] bool can_plant_bomb(int x, int y) const;
    [[nodiscard]] bool can_defuse(const std::shared_ptr<Player>& player) const;
    void clear_bomb();
    void drop_weapon(std::unique_ptr<Gun> gun, int x, int y);
    void drop_bomb(std::unique_ptr<BombEncapsulator> bomb, int x, int y);
    std::unique_ptr<Gun> pick_weapon(int x, int y);
    std::unique_ptr<BombEncapsulator> pick_bomb(int x, int y);
};


#endif  // MAP_H
