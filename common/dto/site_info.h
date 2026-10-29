#ifndef SITE_INFO_H
#define SITE_INFO_H

#include <cstdint>
#include <utility>
#include <vector>

#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"

// Zona de inicio de un equipo con sus puntos de aparicion.
class SiteInfo {
private:
    int32_t x;
    int32_t y;
    int32_t site_width;
    int32_t site_height;
    std::vector<std::pair<int32_t, int32_t>> spawns;

public:
    SiteInfo(int32_t x, int32_t y, int32_t site_width, int32_t site_height,
             std::vector<std::pair<int32_t, int32_t>> spawns):
            x(x),
            y(y),
            site_width(site_width),
            site_height(site_height),
            spawns(std::move(spawns)) {}

    void write(ByteWriter& writer) const {
        writer.i32(x);
        writer.i32(y);
        writer.i32(site_width);
        writer.i32(site_height);
        writer.list_u32(spawns, [](ByteWriter& w, const std::pair<int32_t, int32_t>& spawn) {
            w.i32(spawn.first);
            w.i32(spawn.second);
        });
    }

    static SiteInfo read(ByteReader& reader) {
        const int32_t x = reader.i32();
        const int32_t y = reader.i32();
        const int32_t width = reader.i32();
        const int32_t height = reader.i32();
        auto spawns = reader.list_u32<std::vector<std::pair<int32_t, int32_t>>>([](ByteReader& r) {
            const int32_t spawn_x = r.i32();
            const int32_t spawn_y = r.i32();
            return std::make_pair(spawn_x, spawn_y);
        });
        return SiteInfo(x, y, width, height, std::move(spawns));
    }

    int32_t get_x() const { return x; }

    int32_t get_y() const { return y; }

    int32_t get_width() const { return site_width; }

    int32_t get_height() const { return site_height; }

    const std::vector<std::pair<int32_t, int32_t>>& get_spawns() const { return spawns; }
};

#endif
