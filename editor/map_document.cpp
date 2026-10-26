#include "map_document.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>

#include <yaml-cpp/yaml.h>

bool MapRect::contains(int px, int py, int size) const {
    return px >= x && py >= y && px + size <= x + width && py + size <= y + height;
}

bool MapRect::overlaps(const MapRect& other) const {
    return x < other.x + other.width && other.x < x + width && y < other.y + other.height &&
           other.y < y + height;
}

const std::vector<std::string>& MapDocument::themes() {
    static const std::vector<std::string> themes = {"desert", "aztec_village", "training"};
    return themes;
}

const std::vector<std::string>& MapDocument::weapons() {
    static const std::vector<std::string> weapons = {"ak-47", "m3", "awp"};
    return weapons;
}

namespace {

MapRect read_rect(const YAML::Node& node) {
    return MapRect{node["x"].as<int>(), node["y"].as<int>(), node["width"].as<int>(),
                   node["height"].as<int>()};
}

TeamZone read_zone(const YAML::Node& node) {
    TeamZone zone;
    if (!node) {
        return zone;
    }
    zone.defined = true;
    zone.area = read_rect(node);
    for (const auto& spawn: node["spawns"]) {
        zone.spawns.push_back(MapPoint{spawn["x"].as<int>(), spawn["y"].as<int>()});
    }
    return zone;
}

void write_rect(YAML::Emitter& out, const MapRect& rect) {
    out << YAML::Flow << YAML::BeginMap;
    out << YAML::Key << "x" << YAML::Value << rect.x;
    out << YAML::Key << "y" << YAML::Value << rect.y;
    out << YAML::Key << "width" << YAML::Value << rect.width;
    out << YAML::Key << "height" << YAML::Value << rect.height;
    out << YAML::EndMap;
}

void write_zone(YAML::Emitter& out, const TeamZone& zone) {
    out << YAML::BeginMap;
    out << YAML::Key << "x" << YAML::Value << zone.area.x;
    out << YAML::Key << "y" << YAML::Value << zone.area.y;
    out << YAML::Key << "width" << YAML::Value << zone.area.width;
    out << YAML::Key << "height" << YAML::Value << zone.area.height;
    out << YAML::Key << "spawns" << YAML::Value << YAML::BeginSeq;
    for (const auto& spawn: zone.spawns) {
        out << YAML::Flow << YAML::BeginMap << YAML::Key << "x" << YAML::Value << spawn.x
            << YAML::Key << "y" << YAML::Value << spawn.y << YAML::EndMap;
    }
    out << YAML::EndSeq << YAML::EndMap;
}

MapDocument from_node(const YAML::Node& root) {
    MapDocument map;
    map.name = root["map_name"].as<std::string>();
    map.theme = root["theme"] ? root["theme"].as<std::string>() : "desert";
    map.width = root["map_width"].as<int>();
    map.height = root["map_height"].as<int>();
    for (const auto& structure: root["structures"]) {
        map.structures.push_back(read_rect(structure));
    }
    // Igual que el servidor: lista "bomb_sites" o una sola "bomb_site"
    if (const YAML::Node sites = root["bomb_sites"]) {
        for (const auto& site: sites) {
            map.bomb_sites.push_back(read_rect(site));
        }
    } else if (const YAML::Node site = root["bomb_site"]) {
        map.bomb_sites.push_back(read_rect(site));
    }
    map.tt_zone = read_zone(root["tt_site"]);
    map.ct_zone = read_zone(root["ct_site"]);
    for (const auto& drop: root["weapon_drops"]) {
        map.weapon_drops.push_back(WeaponDrop{drop["name"].as<std::string>(),
                                              MapPoint{drop["x"].as<int>(), drop["y"].as<int>()}});
    }
    return map;
}

bool inside_map(const MapDocument& map, const MapRect& r) {
    return r.x >= 0 && r.y >= 0 && r.width > 0 && r.height > 0 && r.x + r.width <= map.width &&
           r.y + r.height <= map.height;
}

void validate_zone(const MapDocument& map, const TeamZone& zone, const std::string& team,
                   MapValidation& result) {
    if (!zone.defined) {
        result.errors.push_back("Falta la zona de inicio " + team + ".");
        return;
    }
    if (!inside_map(map, zone.area)) {
        result.errors.push_back("La zona de inicio " + team + " se sale del mapa.");
    }
    if (zone.spawns.empty()) {
        result.errors.push_back("La zona de inicio " + team + " no tiene spawns.");
    } else if (static_cast<int>(zone.spawns.size()) < MapDocument::RECOMMENDED_SPAWNS) {
        result.warnings.push_back("La zona " + team + " tiene " +
                                  std::to_string(zone.spawns.size()) + " spawns; con menos de " +
                                  std::to_string(MapDocument::RECOMMENDED_SPAWNS) +
                                  " algunos jugadores no tienen lugar asignado.");
    }
    for (const auto& spawn: zone.spawns) {
        const std::string where =
                "(" + std::to_string(spawn.x) + ", " + std::to_string(spawn.y) + ")";
        if (!zone.area.contains(spawn.x, spawn.y, MapDocument::PLAYER_SIZE)) {
            result.errors.push_back("El spawn " + team + " en " + where +
                                    " queda fuera de su zona.");
        }
        const MapRect footprint{spawn.x, spawn.y, MapDocument::PLAYER_SIZE,
                                MapDocument::PLAYER_SIZE};
        if (std::ranges::any_of(map.structures,
                                [&](const MapRect& s) { return s.overlaps(footprint); })) {
            result.errors.push_back("El spawn " + team + " en " + where +
                                    " esta tapado por una estructura.");
        }
    }
}

}  // namespace

MapDocument MapDocument::load(const std::string& path) {
    try {
        return from_node(YAML::LoadFile(path));
    } catch (const YAML::Exception& e) {
        throw std::runtime_error("No se pudo leer el mapa " + path + ": " + e.what());
    }
}

MapDocument MapDocument::from_yaml(const std::string& text) {
    try {
        return from_node(YAML::Load(text));
    } catch (const YAML::Exception& e) {
        throw std::runtime_error(std::string("Mapa invalido: ") + e.what());
    }
}

std::string MapDocument::to_yaml() const {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "map_name" << YAML::Value << name;
    out << YAML::Key << "theme" << YAML::Value << theme;
    out << YAML::Key << "map_width" << YAML::Value << width;
    out << YAML::Key << "map_height" << YAML::Value << height;
    out << YAML::Key << "structures" << YAML::Value << YAML::BeginSeq;
    for (const auto& structure: structures) {
        write_rect(out, structure);
    }
    out << YAML::EndSeq;
    out << YAML::Key << "bomb_sites" << YAML::Value << YAML::BeginSeq;
    for (const auto& site: bomb_sites) {
        write_rect(out, site);
    }
    out << YAML::EndSeq;
    if (tt_zone.defined) {
        out << YAML::Key << "tt_site" << YAML::Value;
        write_zone(out, tt_zone);
    }
    if (ct_zone.defined) {
        out << YAML::Key << "ct_site" << YAML::Value;
        write_zone(out, ct_zone);
    }
    out << YAML::Key << "weapon_drops" << YAML::Value << YAML::BeginSeq;
    for (const auto& drop: weapon_drops) {
        out << YAML::Flow << YAML::BeginMap << YAML::Key << "name" << YAML::Value << drop.weapon
            << YAML::Key << "x" << YAML::Value << drop.position.x << YAML::Key << "y" << YAML::Value
            << drop.position.y << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
    return std::string("# Mapa \"") + name + "\" creado con el editor de niveles.\n" + out.c_str() +
           "\n";
}

void MapDocument::save(const std::string& path) const {
    std::ofstream file(path);
    if (!file) {
        throw std::runtime_error("No se pudo escribir " + path);
    }
    file << to_yaml();
}

MapValidation MapDocument::validate() const {
    MapValidation result;
    const MapRect bounds{0, 0, width, height};
    auto inside = [this](const MapRect& r) { return inside_map(*this, r); };

    if (name.empty() || name.find_first_of("/\\ ") != std::string::npos) {
        result.errors.push_back("El nombre del mapa no puede estar vacio ni tener espacios o '/'.");
    }
    if (width < MIN_MAP_SIZE || height < MIN_MAP_SIZE) {
        result.errors.push_back("El mapa tiene que medir al menos " + std::to_string(MIN_MAP_SIZE) +
                                "x" + std::to_string(MIN_MAP_SIZE) + " tiles.");
    }
    if (std::ranges::find(themes(), theme) == themes().end()) {
        result.errors.push_back("Terreno desconocido: " + theme + ".");
    }

    validate_zone(*this, tt_zone, "terrorista", result);
    validate_zone(*this, ct_zone, "antiterrorista", result);

    if (bomb_sites.empty()) {
        result.errors.push_back("Tiene que haber al menos una zona para plantar la bomba.");
    }
    for (const auto& site: bomb_sites) {
        if (!inside(site)) {
            result.errors.push_back("Una zona de bomba se sale del mapa.");
        } else if (std::ranges::any_of(structures,
                                       [&](const MapRect& s) { return s.overlaps(site); })) {
            result.warnings.push_back("Una zona de bomba tiene estructuras encima: hay lugares "
                                      "donde no se va a poder plantar.");
        }
    }
    for (const auto& structure: structures) {
        if (!inside(structure)) {
            result.errors.push_back("Hay una estructura que se sale del mapa.");
            break;
        }
    }
    for (const auto& drop: weapon_drops) {
        const MapRect footprint{drop.position.x, drop.position.y, WEAPON_SIZE, WEAPON_SIZE};
        if (std::ranges::find(weapons(), drop.weapon) == weapons().end()) {
            result.errors.push_back("Arma desconocida: " + drop.weapon + ".");
        }
        if (!bounds.contains(drop.position.x, drop.position.y, WEAPON_SIZE)) {
            result.errors.push_back("Hay un arma fuera del mapa.");
        } else if (std::ranges::any_of(structures,
                                       [&](const MapRect& s) { return s.overlaps(footprint); })) {
            result.errors.push_back("Hay un arma (" + drop.weapon +
                                    ") debajo de una estructura: nadie la podria agarrar.");
        }
    }
    return result;
}
