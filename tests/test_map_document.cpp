#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>

#include "editor/map_document.h"
#include "gtest/gtest.h"

#ifdef TALLER_TEST_SERVER_LOADER
#include "server/configuration_loader.h"
#endif

namespace {

const std::string SOURCE_DIR = TALLER_SOURCE_DIR;

// Mapa chico y valido para partir en cada test
MapDocument valid_map() {
    MapDocument map;
    map.name = "prueba";
    map.theme = "training";
    map.width = 60;
    map.height = 60;
    map.structures = {{25, 25, 6, 6}};
    map.bomb_sites = {{20, 2, 20, 10}, {20, 45, 20, 10}};
    map.tt_zone = {true, {2, 20, 14, 20}, {{3, 21}, {9, 21}, {3, 27}, {9, 27}, {3, 33}}};
    map.ct_zone = {true, {44, 20, 14, 20}, {{45, 21}, {51, 21}, {45, 27}, {51, 27}, {45, 33}}};
    map.weapon_drops = {{"awp", {10, 50}}};
    return map;
}

bool has_message(const std::vector<std::string>& messages, const std::string& text) {
    return std::ranges::any_of(
            messages, [&](const std::string& m) { return m.find(text) != std::string::npos; });
}

std::filesystem::path temp_file(const std::string& name) {
    return std::filesystem::temp_directory_path() / ("cs2d_test_" + name);
}

}  // namespace

TEST(MapDocumentTest, ValidMapHasNoErrorsNorWarnings) {
    const MapValidation result = valid_map().validate();
    EXPECT_TRUE(result.ok());
    EXPECT_TRUE(result.warnings.empty());
}

TEST(MapDocumentTest, SaveAndLoadKeepsEverything) {
    const MapDocument map = valid_map();
    const auto path = temp_file("roundtrip.yaml");
    map.save(path.string());
    EXPECT_EQ(MapDocument::load(path.string()), map);
    std::filesystem::remove(path);
}

TEST(MapDocumentTest, RequiresBothStartZones) {
    MapDocument map = valid_map();
    map.ct_zone = {};
    EXPECT_TRUE(has_message(map.validate().errors, "zona de inicio antiterrorista"));
    map.tt_zone = {};
    EXPECT_TRUE(has_message(map.validate().errors, "zona de inicio terrorista"));
}

TEST(MapDocumentTest, RequiresAtLeastOneBombSite) {
    MapDocument map = valid_map();
    map.bomb_sites.clear();
    EXPECT_TRUE(has_message(map.validate().errors, "al menos una zona para plantar"));
}

TEST(MapDocumentTest, SpawnsMustBeInsideTheirZoneAndFree) {
    MapDocument map = valid_map();
    map.tt_zone.spawns.push_back({40, 40});
    EXPECT_TRUE(has_message(map.validate().errors, "fuera de su zona"));

    map = valid_map();
    map.structures.push_back({2, 20, 6, 6});
    EXPECT_TRUE(has_message(map.validate().errors, "tapado por una estructura"));

    map = valid_map();
    map.ct_zone.spawns.clear();
    EXPECT_TRUE(has_message(map.validate().errors, "no tiene spawns"));
}

TEST(MapDocumentTest, FewSpawnsIsOnlyAWarning) {
    MapDocument map = valid_map();
    map.tt_zone.spawns.resize(2);
    const MapValidation result = map.validate();
    EXPECT_TRUE(result.ok());
    EXPECT_TRUE(has_message(result.warnings, "zona terrorista tiene 2 spawns"));
}

TEST(MapDocumentTest, ElementsMustStayInsideTheMap) {
    MapDocument map = valid_map();
    map.structures.push_back({55, 55, 10, 10});
    EXPECT_TRUE(has_message(map.validate().errors, "estructura que se sale"));

    map = valid_map();
    map.weapon_drops.push_back({"m3", {59, 59}});
    EXPECT_TRUE(has_message(map.validate().errors, "arma fuera del mapa"));
}

TEST(MapDocumentTest, WeaponUnderAStructureIsAnError) {
    MapDocument map = valid_map();
    map.weapon_drops.push_back({"ak-47", {26, 26}});
    EXPECT_TRUE(has_message(map.validate().errors, "debajo de una estructura"));
}

TEST(MapDocumentTest, ServerMapsAreValid) {
    for (const std::string name: {"desierto", "pueblo-azteca", "entrenamiento"}) {
        const MapDocument map = MapDocument::load(SOURCE_DIR + "/server/maps/" + name + ".yaml");
        const MapValidation result = map.validate();
        EXPECT_TRUE(result.ok()) << name << ": " << (result.errors.empty() ? "" : result.errors[0]);
        EXPECT_EQ(map.name, name);
    }
}

#ifdef TALLER_TEST_SERVER_LOADER
TEST(MapDocumentTest, ServerLoadsAMapSavedByTheEditor) {
    // Config del servidor que solo lista un mapa guardado por el editor
    const auto map_path = temp_file("editor_map.yaml");
    const auto config_path = temp_file("server_config.yaml");
    valid_map().save(map_path.string());
    std::ifstream original(SOURCE_DIR + "/server/server_config.yaml");
    std::ofstream config(config_path);
    std::string line;
    while (std::getline(original, line) && line.rfind("maps:", 0) != 0) {
        config << line << "\n";
    }
    config << "maps:\n  - " << map_path.filename().string() << "\n";
    config.close();

    const GameConfig game_config = ConfigurationLoader(config_path.string()).load_configuration();
    const MapConfig& map = game_config.find_map("prueba");
    EXPECT_EQ(map.map_name, "prueba");
    EXPECT_EQ(map.theme, "training");
    EXPECT_EQ(map.bomb_sites.size(), 2);
    EXPECT_EQ(map.structures.size(), 1);
    EXPECT_EQ(map.tt_site.spawns.size(), 5);
    EXPECT_EQ(map.dropped_weapons.size(), 1);
    std::filesystem::remove(map_path);
    std::filesystem::remove(config_path);
}
#endif
