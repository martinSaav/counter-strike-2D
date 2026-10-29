#ifndef MAP_CONFIG_INFO_H
#define MAP_CONFIG_INFO_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"

#include "rect_info.h"
#include "site_info.h"

class MapConfigInfo {
private:
    std::string map_name;
    int32_t map_width;
    int32_t map_height;
    std::vector<StructureInfo> structures;
    std::vector<BombSiteInfo> bombsites;
    SiteInfo ct_site;
    SiteInfo tt_site;
    // Estilo visual del mapa (carpeta de texturas en el cliente)
    std::string theme;

public:
    MapConfigInfo(std::string map_name, int32_t map_width, int32_t map_height,
                  std::vector<StructureInfo> structures, std::vector<BombSiteInfo> bombsites,
                  SiteInfo ct_site, SiteInfo tt_site, std::string theme = "desert"):
            map_name(std::move(map_name)),
            map_width(map_width),
            map_height(map_height),
            structures(std::move(structures)),
            bombsites(std::move(bombsites)),
            ct_site(std::move(ct_site)),
            tt_site(std::move(tt_site)),
            theme(std::move(theme)) {}

    void write(ByteWriter& writer) const {
        const auto write_rect = [](ByteWriter& w, const RectInfo& rect) { rect.write(w); };
        writer.string(map_name);
        writer.i32(map_width);
        writer.i32(map_height);
        writer.list_u32(structures, write_rect);
        writer.list_u32(bombsites, write_rect);
        ct_site.write(writer);
        tt_site.write(writer);
        writer.string(theme);
    }

    static MapConfigInfo read(ByteReader& reader) {
        std::string map_name = reader.string();
        const int32_t map_width = reader.i32();
        const int32_t map_height = reader.i32();
        auto structures = reader.list_u32<std::vector<StructureInfo>>(RectInfo::read);
        auto bombsites = reader.list_u32<std::vector<BombSiteInfo>>(RectInfo::read);
        SiteInfo ct_site = SiteInfo::read(reader);
        SiteInfo tt_site = SiteInfo::read(reader);
        std::string theme = reader.string();
        return MapConfigInfo(std::move(map_name), map_width, map_height, std::move(structures),
                             std::move(bombsites), std::move(ct_site), std::move(tt_site),
                             std::move(theme));
    }

    const std::string& get_map_name() const { return map_name; }

    int32_t get_map_width() const { return map_width; }

    int32_t get_map_height() const { return map_height; }

    const std::vector<StructureInfo>& get_structures() const { return structures; }

    const std::vector<BombSiteInfo>& get_bombsites() const { return bombsites; }

    const SiteInfo& get_ct_site() const { return ct_site; }

    const SiteInfo& get_tt_site() const { return tt_site; }

    const std::string& get_theme() const { return theme; }
};

#endif
