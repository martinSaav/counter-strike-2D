#ifndef MAP_DOCUMENT_H
#define MAP_DOCUMENT_H

#include <string>
#include <vector>

// Modelo del mapa que edita el editor de niveles. No depende de Qt: lee y escribe el mismo YAML
// que carga el servidor (ConfigurationLoader::get_map_config) y valida que el mapa sea jugable.
// Todas las medidas estan en tiles.

struct MapRect {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;

    [[nodiscard]] bool contains(int px, int py, int size) const;
    [[nodiscard]] bool overlaps(const MapRect& other) const;
    bool operator==(const MapRect& other) const = default;
};

struct MapPoint {
    int x = 0;
    int y = 0;
    bool operator==(const MapPoint& other) const = default;
};

// Zona de inicio de un equipo: el area y los puntos donde aparecen los jugadores.
struct TeamZone {
    bool defined = false;
    MapRect area;
    std::vector<MapPoint> spawns;
    bool operator==(const TeamZone& other) const = default;
};

struct WeaponDrop {
    std::string weapon;  // "ak-47", "m3" o "awp"
    MapPoint position;
    bool operator==(const WeaponDrop& other) const = default;
};

struct MapValidation {
    std::vector<std::string> errors;    // impiden guardar
    std::vector<std::string> warnings;  // se puede guardar igual

    [[nodiscard]] bool ok() const { return errors.empty(); }
};

class MapDocument {
public:
    // Tamano en tiles de lo que ocupa un jugador en un spawn y un arma tirada en el piso
    static constexpr int PLAYER_SIZE = 4;
    static constexpr int WEAPON_SIZE = 3;
    static constexpr int RECOMMENDED_SPAWNS = 5;
    static constexpr int MIN_MAP_SIZE = 20;
    static const std::vector<std::string>& themes();
    static const std::vector<std::string>& weapons();

    std::string name = "nuevo-mapa";
    std::string theme = "desert";
    int width = 100;
    int height = 100;
    std::vector<MapRect> structures;
    std::vector<MapRect> bomb_sites;
    TeamZone tt_zone;
    TeamZone ct_zone;
    std::vector<WeaponDrop> weapon_drops;

    static MapDocument load(const std::string& path);
    static MapDocument from_yaml(const std::string& text);
    void save(const std::string& path) const;
    [[nodiscard]] std::string to_yaml() const;

    [[nodiscard]] MapValidation validate() const;

    bool operator==(const MapDocument& other) const = default;
};

#endif  // MAP_DOCUMENT_H
