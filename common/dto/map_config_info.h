#ifndef MAP_CONFIG_INFO_H
#define MAP_CONFIG_INFO_H

#include <cstdint>
#include <cstring>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "bomb_site_info.h"
#include "site_info.h"
#include "structure_info.h"

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

    void serialize(uint8_t* buffer) const {
        size_t offset = 0;

        uint16_t name_len = htons(static_cast<uint16_t>(map_name.size()));
        std::memcpy(buffer + offset, &name_len, sizeof(name_len));
        offset += sizeof(name_len);

        std::memcpy(buffer + offset, map_name.data(), map_name.size());
        offset += map_name.size();

        int32_t width_net = htonl(map_width);
        std::memcpy(buffer + offset, &width_net, sizeof(width_net));
        offset += sizeof(width_net);

        int32_t height_net = htonl(map_height);
        std::memcpy(buffer + offset, &height_net, sizeof(height_net));
        offset += sizeof(height_net);

        int32_t struct_count = htonl(static_cast<int32_t>(structures.size()));
        std::memcpy(buffer + offset, &struct_count, sizeof(struct_count));
        offset += sizeof(struct_count);

        for (const auto& s: structures) {
            s.serialize(buffer + offset);
            offset += s.serialized_size();
        }

        int32_t bombsite_count = htonl(static_cast<int32_t>(bombsites.size()));
        std::memcpy(buffer + offset, &bombsite_count, sizeof(bombsite_count));
        offset += sizeof(bombsite_count);
        for (const auto& bombsite: bombsites) {
            bombsite.serialize(buffer + offset);
            offset += bombsite.serialized_size();
        }

        ct_site.serialize(buffer + offset);
        offset += ct_site.serialized_size();

        tt_site.serialize(buffer + offset);
        offset += tt_site.serialized_size();

        uint16_t theme_len = htons(static_cast<uint16_t>(theme.size()));
        std::memcpy(buffer + offset, &theme_len, sizeof(theme_len));
        offset += sizeof(theme_len);
        std::memcpy(buffer + offset, theme.data(), theme.size());
    }

    static MapConfigInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }

        size_t offset = 0;

        uint16_t name_len;
        std::memcpy(&name_len, buffer + offset, sizeof(name_len));
        name_len = ntohs(name_len);
        offset += sizeof(name_len);

        std::string map_name(reinterpret_cast<const char*>(buffer + offset), name_len);
        offset += name_len;

        int32_t map_width;
        std::memcpy(&map_width, buffer + offset, sizeof(map_width));
        map_width = ntohl(map_width);
        offset += sizeof(map_width);

        int32_t map_height;
        std::memcpy(&map_height, buffer + offset, sizeof(map_height));
        map_height = ntohl(map_height);
        offset += sizeof(map_height);

        int32_t struct_count;
        std::memcpy(&struct_count, buffer + offset, sizeof(struct_count));
        struct_count = ntohl(struct_count);
        offset += sizeof(struct_count);

        std::vector<StructureInfo> structures;
        structures.reserve(struct_count);
        for (int32_t i = 0; i < struct_count; ++i) {
            structures.push_back(StructureInfo::deserialize(buffer + offset, size - offset));
            offset += structures.back().serialized_size();
        }

        int32_t bombsite_count;
        std::memcpy(&bombsite_count, buffer + offset, sizeof(bombsite_count));
        bombsite_count = ntohl(bombsite_count);
        offset += sizeof(bombsite_count);
        std::vector<BombSiteInfo> bombsites;
        for (int32_t i = 0; i < bombsite_count; ++i) {
            bombsites.push_back(BombSiteInfo::deserialize(buffer + offset, size - offset));
            offset += bombsites.back().serialized_size();
        }

        SiteInfo ct_site = SiteInfo::deserialize(buffer + offset, size - offset);
        offset += ct_site.serialized_size();

        SiteInfo tt_site = SiteInfo::deserialize(buffer + offset, size - offset);
        offset += tt_site.serialized_size();

        uint16_t theme_len;
        std::memcpy(&theme_len, buffer + offset, sizeof(theme_len));
        theme_len = ntohs(theme_len);
        offset += sizeof(theme_len);
        std::string theme(reinterpret_cast<const char*>(buffer + offset), theme_len);

        return MapConfigInfo(std::move(map_name), map_width, map_height, std::move(structures),
                             std::move(bombsites), std::move(ct_site), std::move(tt_site),
                             std::move(theme));
    }

    size_t serialized_size() const {
        size_t total = sizeof(uint16_t) + map_name.size();  // name length + name
        total += sizeof(int32_t) * 3;                       // width, height, count
        total += std::accumulate(structures.begin(), structures.end(), size_t(0),
                                 [](size_t acc, const auto& structure) {
                                     return acc + structure.serialized_size();
                                 });
        total += sizeof(int32_t);  // cantidad de zonas de bomba
        total += std::accumulate(
                bombsites.begin(), bombsites.end(), size_t(0),
                [](size_t acc, const auto& site) { return acc + site.serialized_size(); });
        total += ct_site.serialized_size();
        total += tt_site.serialized_size();
        total += sizeof(uint16_t) + theme.size();  // theme length + theme
        return total;
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
