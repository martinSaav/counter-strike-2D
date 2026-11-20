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
    for (const auto& site: config.map_config.bomb_sites) {
        add_bombsite(site);
    }
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
    // Las estructuras pueden superponerse o tocarse: para las colisiones no hace falta que sean
    // disjuntas. Antes se rechazaban (y el constructor tragaba la excepcion), asi que el servidor
    // perdia paredes que el cliente igual dibujaba (por ejemplo una caja de pueblo-azteca). Se
    // agregan a todos los chunks que tocan, recortados al mapa: tomar solo las cuatro esquinas
    // dejaba afuera los chunks del medio y una pared que se salia del mapa se perdia entera.
    const int last_valid_chunk_x = max_x / CHUNK_SIZE;
    const int last_valid_chunk_y = max_y / CHUNK_SIZE;
    const int first_chunk_x = x / CHUNK_SIZE;
    const int first_chunk_y = y / CHUNK_SIZE;
    const int last_chunk_x = std::min((x + structure.width) / CHUNK_SIZE, last_valid_chunk_x);
    const int last_chunk_y = std::min((y + structure.height) / CHUNK_SIZE, last_valid_chunk_y);
    for (int chunk_x = first_chunk_x; chunk_x <= last_chunk_x; chunk_x++) {
        for (int chunk_y = first_chunk_y; chunk_y <= last_chunk_y; chunk_y++) {
            structure_chunks[std::make_pair(chunk_x, chunk_y)].add_structure(structure);
        }
    }
}


void Map::add_player(const std::shared_ptr<Player>& player) { players.emplace_back(player); }


bool Map::check_if_position_is_in_range(const int x, const int y) const {
    return x >= 0 && x <= max_x && y >= 0 && y <= max_y;
}


std::vector<Structure> Map::get_structures_near_player(int x, int y) {
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


std::vector<std::shared_ptr<Player>> Map::get_players_in_chunk(
        const std::pair<int, int>& chunk_index) const {
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
                                                const Position& final_pos) {
    int nearest_structure_distance = -1;
    std::pair<int, int> collision_point;
    auto [final_x, final_y] = final_pos.get_position();
    for (const auto& structure: chunk.get_structures()) {
        const auto impact =
                CollisionDetector::get_bullet_impact(structure, ini_x, ini_y, final_x, final_y);
        if (!impact.has_value()) {
            continue;
        }
        auto [imp_x, imp_y] = impact.value();
        if (int structure_dis = sqrt(std::pow(imp_x - ini_x, 2) + std::pow(imp_y - ini_y, 2));
            structure_dis < nearest_structure_distance || nearest_structure_distance == -1) {
            nearest_structure_distance = structure_dis;
            collision_point = std::make_pair(imp_x, imp_y);
        }
    }
    if (nearest_structure_distance != -1) {
        return ImpactInfo{nearest_structure_distance, collision_point};
    }
    return ImpactInfo{};
}


ImpactInfo Map::get_nearest_colliding_player(const std::pair<int, int>& chunk_idx, int ini_x,
                                             int ini_y, const Player& gun_owner,
                                             const Position& final_pos) const {
    auto [final_x, final_y] = final_pos.get_position();
    int nearest_player_distance = -1;
    std::pair<int, int> nearest_player_pos;
    std::shared_ptr<Player> nearest_player = nullptr;
    for (const auto& player: get_players_in_chunk(chunk_idx)) {
        if (player->get_username() == gun_owner.get_username()) {
            continue;
        }
        auto [x, y] = player->get_location();
        const auto impact =
                CollisionDetector::get_bullet_impact(x, y, ini_x, ini_y, final_x, final_y);
        if (!impact.has_value()) {
            continue;
        }
        auto [imp_x, imp_y] = impact.value();
        if (int player_dis = sqrt(std::pow(imp_x - ini_x, 2) + std::pow(imp_y - ini_y, 2));
            player_dis < nearest_player_distance || nearest_player_distance == -1) {
            nearest_player_distance = player_dis;
            nearest_player = player;
            nearest_player_pos = std::make_pair(imp_x, imp_y);
        }
    }
    if (nearest_player_distance != -1) {
        return ImpactInfo{nearest_player_distance, nearest_player_pos, nearest_player};
    }
    return ImpactInfo{};
}


std::optional<ImpactInfo> Map::get_impacted_element(const std::pair<int, int>& chunk_idx, int ini_x,
                                                    int ini_y, const Position& final_pos,
                                                    const Player& gun_owner) {
    int nearest_structure_distance = -1;
    std::pair<int, int> nearest_structure_pos;
    if (auto chunk_p = structure_chunks.find(chunk_idx); chunk_p != structure_chunks.end()) {
        const Chunk& chunk = chunk_p->second;
        ImpactInfo structure_hit = get_nearest_colliding_structure(chunk, ini_x, ini_y, final_pos);
        if (structure_hit.hit) {
            nearest_structure_distance = structure_hit.distance;
            nearest_structure_pos = structure_hit.impact_position;
        }
    }
    ImpactInfo player_impacted =
            get_nearest_colliding_player(chunk_idx, ini_x, ini_y, gun_owner, final_pos);
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
    auto [final_x, final_y] = final_pos.get_position();
    const int final_pos_distance = sqrt(pow(final_x - ini_x, 2) + pow(final_y - ini_y, 2));
    // Se revisan todos los chunks que toca la caja del segmento y gana el impacto mas cercano.
    // Antes se muestreaba un punto cada CHUNK_SIZE y en los disparos diagonales quedaban sin
    // revisar los chunks de las esquinas, por donde la bala pasaba sin pegarle a nadie. Los chunks
    // son grandes y un mapa tiene pocos, asi que revisarlos todos es barato. Los indices se
    // recortan al mapa, asi que apuntar afuera ya no lanza PositionOutOfRange.
    const int last_valid_chunk_x = max_x / CHUNK_SIZE;
    const int last_valid_chunk_y = max_y / CHUNK_SIZE;
    const int first_chunk_x =
            std::clamp(std::min(ini_x, final_x) / CHUNK_SIZE, 0, last_valid_chunk_x);
    const int last_chunk_x =
            std::clamp(std::max(ini_x, final_x) / CHUNK_SIZE, 0, last_valid_chunk_x);
    const int first_chunk_y =
            std::clamp(std::min(ini_y, final_y) / CHUNK_SIZE, 0, last_valid_chunk_y);
    const int last_chunk_y =
            std::clamp(std::max(ini_y, final_y) / CHUNK_SIZE, 0, last_valid_chunk_y);
    std::optional<ImpactInfo> nearest;
    for (int chunk_x = first_chunk_x; chunk_x <= last_chunk_x; chunk_x++) {
        for (int chunk_y = first_chunk_y; chunk_y <= last_chunk_y; chunk_y++) {
            std::optional<ImpactInfo> impacted = get_impacted_element(
                    std::make_pair(chunk_x, chunk_y), ini_x, ini_y, final_pos, gun_owner);
            if (impacted.has_value() &&
                (!nearest.has_value() || impacted->distance < nearest->distance)) {
                nearest.reset();  // ImpactInfo tiene miembros const: no se puede asignar
                nearest.emplace(impacted.value());
            }
        }
    }
    if (nearest.has_value()) {
        return nearest.value();
    }
    return ImpactInfo{final_pos_distance, final_pos.get_position()};
}


std::vector<std::shared_ptr<Player>> Map::get_players_near_radio(const double x, const double y,
                                                                 const double radio) const {
    std::vector<std::shared_ptr<Player>> result;

    for (const auto& player: players) {
        if (player->is_dead()) {
            continue;
        }
        auto [center_x, center_y] = player->get_center_coordinates();
        const double dis_x = center_x - x;
        const double dis_y = center_y - y;

        if (const double distance = sqrt(dis_x * dis_x + dis_y * dis_y); distance <= radio) {
            result.push_back(player);
        }
    }

    return result;
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


std::unique_ptr<Gun> Map::pick_weapon(int x, int y, const bool can_pick_primary) {
    auto it = dropped_guns.begin();
    while (it != dropped_guns.end()) {
        if (it->second == nullptr) {
            it = dropped_guns.erase(it);
            continue;
        }
        if (!can_pick_primary && it->second->get_gun_type() == GunType::Primary) {
            ++it;
            continue;
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
            continue;
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
            it = players.erase(it);
            continue;
        }
        ++it;
    }
}


Site& Map::get_ct_site() { return ct_site; }


Site& Map::get_tt_site() { return tt_site; }
