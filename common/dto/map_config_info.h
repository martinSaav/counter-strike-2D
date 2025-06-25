#ifndef MAP_CONFIG_INFO_H
#define MAP_CONFIG_INFO_H

#include <cstdint>
#include <cstring>
#include <string>
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
    BombSiteInfo bombsite;
    SiteInfo ct_site;
    SiteInfo tt_site;

public:
    MapConfigInfo(std::string map_name, int32_t map_width, int32_t map_height,
                  std::vector<StructureInfo> structures, BombSiteInfo bombsite, SiteInfo ct_site,
                  SiteInfo tt_site):
            map_name(std::move(map_name)),
            map_width(map_width),
            map_height(map_height),
            structures(std::move(structures)),
            bombsite(std::move(bombsite)),
            ct_site(std::move(ct_site)),
            tt_site(std::move(tt_site)) {}

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

        bombsite.serialize(buffer + offset);
        offset += bombsite.serialized_size();

        ct_site.serialize(buffer + offset);
        offset += ct_site.serialized_size();

        tt_site.serialize(buffer + offset);
        offset += tt_site.serialized_size();
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

        BombSiteInfo bombsite = BombSiteInfo::deserialize(buffer + offset, size - offset);
        offset += bombsite.serialized_size();

        SiteInfo ct_site = SiteInfo::deserialize(buffer + offset, size - offset);
        offset += ct_site.serialized_size();

        SiteInfo tt_site = SiteInfo::deserialize(buffer + offset, size - offset);

        return MapConfigInfo(std::move(map_name), map_width, map_height, std::move(structures),
                             std::move(bombsite), std::move(ct_site), std::move(tt_site));
    }

    size_t serialized_size() const {
        size_t total = sizeof(uint16_t) + map_name.size();  // name length + name
        total += sizeof(int32_t) * 3;                       // width, height, count
        for (const auto& s: structures) {
            total += s.serialized_size();
        }
        total += bombsite.serialized_size();
        total += ct_site.serialized_size();
        total += tt_site.serialized_size();
        return total;
    }

    const std::string& get_map_name() const { return map_name; }

    int32_t get_map_width() const { return map_width; }

    int32_t get_map_height() const { return map_height; }

    const std::vector<StructureInfo>& get_structures() const { return structures; }

    const BombSiteInfo& get_bombsite() const { return bombsite; }

    const SiteInfo& get_ct_site() const { return ct_site; }

    const SiteInfo& get_tt_site() const { return tt_site; }
};

#endif
