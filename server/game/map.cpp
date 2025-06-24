#include "map.h"

#include <algorithm>
#include <cmath>
#include <set>

#include "collision_detector.h"
#include "gun_shop.h"


Map::Map(const GameConfig& config):
        ct_site(config.map_config.ct_site),
        tt_site(config.map_config.tt_site),
        dropped_bomb({Position(0, 0), nullptr}),
        max_x(config.map_config.map_width),
        max_y(config.map_config.map_height) {
    for (const auto structure: config.map_config.structures) {
        try {
            add_structure(structure);
        } catch (...) {}
    }
    add_bombsite(config.map_config.bombsite);
    for (auto [position, weapon]: config.map_config.dropped_weapons) {
        try {
            std::unique_ptr<Gun> gun = GunShop::get_gun(weapon, config);
            dropped_guns.emplace_back(position, std::move(gun));
        } catch (const CantBuyWeapon&) {}
    }
}


std::pair<int, int> Map::get_chunk_index(int x, int y) const {
    if (!check_if_position_is_in_range(x, y)) {
        throw PositionOutOfRange();
    }
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


void Map::add_player(const std::shared_ptr<Player>& player) { players.emplace_back(player); }


bool Map::check_if_position_is_in_range(const int x, const int y) const {
    return x >= 0 && x <= max_x && y >= 0 && y <= max_y;
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


std::vector<std::pair<int, int>> Map::calculate_player_chunks(int bottom_x, int bottom_y) const {
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
        if (other_player == player || other_player->is_dead()) {
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


std::pair<double, double> Map::calculate_bullet_velocity(const std::pair<int, int>& starting_pos,
                                                         const std::pair<int, int>& ending_pos) {
    int dx = ending_pos.first - starting_pos.first;
    int dy = ending_pos.second - starting_pos.second;
    double mod = std::sqrt(dx * dx + dy * dy);
    const double vx = dx / mod;
    const double vy = dy / mod;

    double vel_x;
    double vel_y;
    double chunk_size_double = CHUNK_SIZE;
    if (std::abs(vx) > std::abs(vy)) {
        const double step = chunk_size_double / std::abs(vx);
        vel_x = step * vx;
        vel_y = step * vy;
    } else {
        const double step = chunk_size_double / std::abs(vy);
        vel_x = step * vx;
        vel_y = step * vy;
    }

    return std::make_pair(vel_x, vel_y);
}


std::pair<std::pair<int, int>, std::pair<double, double>> Map::calculate_new_bullet_position(
        const std::pair<double, double>& starting_pos,
        const std::pair<double, double>& velocity) const {
    const double ini_x = starting_pos.first;
    const double ini_y = starting_pos.second;
    double new_x = ini_x + velocity.first;
    double new_y = ini_y + velocity.second;
    std::pair<int, int> new_chunk =
            get_chunk_index(static_cast<int>(new_x), static_cast<int>(new_y));
    return std::make_pair(new_chunk, std::make_pair(new_x, new_y));
}


std::vector<std::shared_ptr<Player>> Map::get_players_near_point(const int x, const int y) const {
    const std::pair<int, int> chunk_index = get_chunk_index(x, y);
    std::vector<std::shared_ptr<Player>> near_players;
    for (const auto& player: players) {
        if (player->is_dead()) {
            continue;
        }
        if (std::vector<std::pair<int, int>>& player_chunks = player->get_chunk_idxs();
            std::ranges::find(player_chunks, chunk_index) != player_chunks.end()) {
            near_players.emplace_back(player);
        }
    }
    return near_players;
}


ImpactInfo Map::get_nearest_colliding_structure(const Chunk& chunk, int ini_x, int ini_y,
                                                const Position& final_pos,
                                                const std::pair<double, double>& velocity) {
    int nearest_structure_distance = -1;
    std::pair<int, int> collision_point;
    auto [final_x, final_y] = final_pos.get_position();
    for (const auto& structure: chunk.get_structures()) {
        if (CollisionDetector::check_collision_between_structure_and_bullet(structure, ini_x, ini_y,
                                                                            final_x, velocity)) {
            auto [imp_x, imp_y] =
                    CollisionDetector::get_impact_point(structure, ini_x, ini_y, final_x, velocity);
            if (int structure_dis = sqrt(std::pow(imp_x - ini_x, 2) + std::pow(imp_y - ini_y, 2));
                structure_dis < nearest_structure_distance || nearest_structure_distance == -1) {
                nearest_structure_distance = structure_dis;
                collision_point = std::make_pair(imp_x, imp_y);
            }
        }
    }
    if (nearest_structure_distance != -1) {
        return ImpactInfo{nearest_structure_distance, collision_point};
    }
    return ImpactInfo{};
}


ImpactInfo Map::get_nearest_colliding_player(int ini_x, int ini_y, double current_x,
                                             double current_y, const Player& gun_owner,
                                             const Position& final_pos,
                                             const std::pair<double, double>& velocity) const {
    auto [final_x, final_y] = final_pos.get_position();
    int nearest_player_distance = -1;
    std::pair<int, int> nearest_player_pos;
    std::shared_ptr<Player> nearest_player = nullptr;
    for (auto players = get_players_near_point(current_x, current_y); const auto& player: players) {
        if (player->get_username() == gun_owner.get_username()) {
            continue;
        }
        if (auto [x, y] = player->get_location();
            CollisionDetector::check_collision_between_player_and_bullet(x, y, ini_x, ini_y,
                                                                         final_x, velocity)) {
            auto [imp_x, imp_y] =
                    CollisionDetector::get_impact_point(x, y, ini_x, ini_y, final_x, velocity);
            if (int player_dis = sqrt(std::pow(imp_x - ini_x, 2) + std::pow(imp_y - ini_y, 2));
                player_dis < nearest_player_distance || nearest_player_distance == -1) {
                nearest_player_distance = player_dis;
                nearest_player = player;
                nearest_player_pos = std::make_pair(imp_x, imp_y);
            }
        }
    }
    if (nearest_player_distance != -1) {
        return ImpactInfo{nearest_player_distance, nearest_player_pos, nearest_player};
    }
    return ImpactInfo{};
}


std::optional<ImpactInfo> Map::get_impacted_element(const std::pair<int, int>& chunk_idx, int ini_x,
                                                    int ini_y, double current_x, double current_y,
                                                    const Position& final_pos,
                                                    const std::pair<double, double>& velocity,
                                                    const Player& gun_owner) {
    int nearest_structure_distance = -1;
    std::pair<int, int> nearest_structure_pos;
    if (auto chunk_p = structure_chunks.find(chunk_idx); chunk_p != structure_chunks.end()) {
        Chunk& chunk = chunk_p->second;
        ImpactInfo structure_hit =
                get_nearest_colliding_structure(chunk, ini_x, ini_y, final_pos, velocity);
        if (structure_hit.hit) {
            nearest_structure_distance = structure_hit.distance;
            nearest_structure_pos = structure_hit.impact_position;
        }
    }
    ImpactInfo player_impacted = get_nearest_colliding_player(ini_x, ini_y, current_x, current_y,
                                                              gun_owner, final_pos, velocity);
    if (player_impacted.hit) {
        if (player_impacted.distance < nearest_structure_distance ||
            nearest_structure_distance == -1) {
            return player_impacted;
        }
        return ImpactInfo{nearest_structure_distance, nearest_structure_pos};
    }
    if (nearest_structure_distance != -1) {
        return ImpactInfo{nearest_structure_distance, nearest_structure_pos};
    }
    return std::nullopt;
}


ImpactInfo Map::trace_bullet_path(int ini_x, int ini_y, const Position final_pos,
                                  const Player& gun_owner) {
    double current_x = ini_x;
    double current_y = ini_y;
    std::pair<int, int> chunk_idx = get_chunk_index(ini_x, ini_y);
    const std::pair<int, int> final_pos_v = final_pos.get_position();
    auto [final_x, final_y] = final_pos_v;
    const std::pair<int, int> final_chunk = get_chunk_index(final_x, final_y);
    const std::pair<double, double> velocity =
            calculate_bullet_velocity({ini_x, ini_y}, final_pos_v);
    const int final_pos_distance = sqrt(pow(final_x - ini_x, 2) + pow(final_y - ini_y, 2));
    int current_distance = 0;
    while (current_distance < final_pos_distance) {
        try {
            auto impacted = get_impacted_element(chunk_idx, ini_x, ini_y, current_x, current_y,
                                                 final_pos, velocity, gun_owner);
            if (impacted.has_value()) {
                return impacted.value();
            }
            auto [new_chunk_idx, new_pos] =
                    calculate_new_bullet_position(std::make_pair(current_x, current_y), velocity);
            chunk_idx = new_chunk_idx;
            current_x = new_pos.first;
            current_y = new_pos.second;
            current_distance = sqrt(pow(current_x - ini_x, 2) + pow(current_y - ini_y, 2));
        } catch (const PositionOutOfRange&) {
            break;
        }
    }
    auto impacted = get_impacted_element(final_chunk, ini_x, ini_y, final_x, final_y, final_pos,
                                         velocity, gun_owner);
    if (impacted.has_value()) {
        return impacted.value();
    }
    return ImpactInfo{final_pos_distance, final_pos.get_position()};
}


std::vector<std::shared_ptr<Player>> Map::get_players_near_radio(const double x, const double y,
                                                                 const double radio) const {
    std::vector<std::shared_ptr<Player>> resultado;

    for (const auto& player: players) {
        if (player->is_dead()) {
            continue;
        }
        auto [center_x, center_y] = player->get_center_coordinates();
        const double dis_x = center_x - x;
        const double dis_y = center_y - y;

        if (const double distance = sqrt(dis_x * dis_x + dis_y * dis_y); distance <= radio) {
            resultado.push_back(player);
        }
    }

    return resultado;
}


void Map::add_bombsite(const BombSite site) { bomb_sites.push_back(site); }


void Map::plant_bomb(int x, int y) { bomb_position = std::make_pair(x, y); }


bool Map::can_plant_bomb(int x, int y) const {
    return std::ranges::any_of(bomb_sites.begin(), bomb_sites.end(),
                               [&](const auto& site) { return site.is_position_inside(x, y); });
}


bool Map::can_defuse(const std::shared_ptr<Player>& player) const {
    if (!bomb_position.has_value()) {
        return false;
    }
    const Position bomb_p(bomb_position.value().first, bomb_position.value().second);
    const Structure bomb_hitbox(bomb_hitbox_height, bomb_hitbox_width, bomb_p);
    if (auto [x, y] = player->get_location();
        CollisionDetector::check_collision_between_player_and_structure(x, y, bomb_hitbox)) {
        return true;
    }
    return false;
}


void Map::clear_bomb() {
    bomb_position = std::nullopt;
    dropped_bomb.second = nullptr;
}


void Map::drop_bomb(std::unique_ptr<BombEncapsulator> bomb, const int x, const int y) {
    Position pos(x, y);
    dropped_bomb = std::make_pair(pos, std::move(bomb));
}


void Map::drop_weapon(std::unique_ptr<Gun> gun, const int x, const int y) {
    Position pos(x, y);
    auto pair = std::make_pair(pos, std::move(gun));
    dropped_guns.push_back(std::move(pair));
}


std::unique_ptr<Gun> Map::pick_weapon(int x, int y) {
    auto it = dropped_guns.begin();
    while (it != dropped_guns.end()) {
        if (it->second == nullptr) {
            it = dropped_guns.erase(it);
        }
        if (const Structure gun_hitbox(gun_hitbox_height, gun_hitbox_width, it->first);
            CollisionDetector::check_collision_between_player_and_structure(x, y, gun_hitbox)) {
            std::unique_ptr<Gun> gun = std::move(it->second);
            dropped_guns.erase(it);
            return gun;
        }
        ++it;
    }
    return nullptr;
}


std::unique_ptr<BombEncapsulator> Map::pick_bomb(const int x, const int y) {
    if (dropped_bomb.second == nullptr) {
        return nullptr;
    }
    if (const Structure bomb_hitbox(bomb_hitbox_height, bomb_hitbox_width, dropped_bomb.first);
        CollisionDetector::check_collision_between_player_and_structure(x, y, bomb_hitbox)) {
        std::unique_ptr<BombEncapsulator> bomb = std::move(dropped_bomb.second);
        dropped_bomb.second = nullptr;
        return bomb;
    }
    return nullptr;
}


std::list<DroppedWeapon> Map::get_dropped_weapons() {
    std::list<DroppedWeapon> weapons;
    auto it = dropped_guns.begin();
    while (it != dropped_guns.end()) {
        if (it->second == nullptr) {
            it = dropped_guns.erase(it);
        }
        auto [x, y] = it->first.get_position();
        weapons.emplace_back(it->second->get_weapon_name().weapon_type, x, y);
        ++it;
    }
    if (dropped_bomb.second != nullptr) {
        auto [x, y] = dropped_bomb.first.get_position();
        weapons.emplace_back(dropped_bomb.second->get_weapon_name().weapon_type, x, y);
    }
    return weapons;
}


void Map::remove_player(const std::shared_ptr<Player>& player) {
    auto it = players.begin();
    while (it != players.end()) {
        if (*it == nullptr) {
            it = players.erase(it);
            continue;
        }
        if (*it == player) {
            players.erase(it);
            continue;
        }
        ++it;
    }
}


Site& Map::get_ct_site() { return ct_site; }


Site& Map::get_tt_site() { return tt_site; }
