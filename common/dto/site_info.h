#ifndef SITE_INFO_H
#define SITE_INFO_H

#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstring>
#include <arpa/inet.h>

class SiteInfo {
private:
    int32_t x;
    int32_t y;
    int32_t site_width;
    int32_t site_height;
    std::vector<std::pair<int32_t, int32_t>> spawns;

public:
    SiteInfo(int32_t x, int32_t y, int32_t site_width, int32_t site_height,
            std::vector<std::pair<int32_t, int32_t>> spawns)
        : x(x), y(y), site_width(site_width), site_height(site_height), spawns(std::move(spawns)) {}
        

    void serialize(uint8_t* buffer) const {
        size_t offset = 0;

        int32_t x_net = htonl(x);
        memcpy(buffer + offset, &x_net, sizeof(x_net));
        offset += sizeof(x_net);

        int32_t y_net = htonl(y);
        memcpy(buffer + offset, &y_net, sizeof(y_net));
        offset += sizeof(y_net);

        int32_t site_width_net = htonl(site_width);
        memcpy(buffer + offset, &site_width_net, sizeof(site_width_net));
        offset += sizeof(site_width_net);

        int32_t site_height_net = htonl(site_height);
        memcpy(buffer + offset, &site_height_net, sizeof(site_height_net));
        offset += sizeof(site_height_net);

        int32_t num_spawns = htonl(static_cast<int32_t>(spawns.size()));
        memcpy(buffer + offset, &num_spawns, sizeof(num_spawns));
        offset += sizeof(num_spawns);

        for (const auto& spawn : spawns) {
            int32_t spawn_x = htonl(spawn.first);
            memcpy(buffer + offset, &spawn_x, sizeof(spawn_x));
            offset += sizeof(spawn_x);
            int32_t spawn_y = htonl(spawn.second);
            memcpy(buffer + offset, &spawn_y, sizeof(spawn_y));
            offset += sizeof(spawn_y);
        }
    }

    static SiteInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
        }
        size_t offset = 0;

        int32_t x_net;
        memcpy(&x_net, buffer + offset, sizeof(x_net));
        x_net = ntohl(x_net);
        offset += sizeof(x_net);

        int32_t y_net;
        memcpy(&y_net, buffer + offset, sizeof(y_net));
        y_net = ntohl(y_net);
        offset += sizeof(y_net);

        int32_t site_width_net;
        memcpy(&site_width_net, buffer + offset, sizeof(site_width_net));
        site_width_net = ntohl(site_width_net);
        offset += sizeof(site_width_net);

        int32_t site_height_net;
        memcpy(&site_height_net, buffer + offset, sizeof(site_height_net));
        site_height_net = ntohl(site_height_net);
        offset += sizeof(site_height_net);

        int32_t num_spawns_net;
        memcpy(&num_spawns_net, buffer + offset, sizeof(num_spawns_net));
        int32_t num_spawns = ntohl(num_spawns_net);
        offset += sizeof(num_spawns_net);   

        std::vector<std::pair<int32_t, int32_t>> spawns;
        spawns.reserve(num_spawns);

        for (int32_t i = 0; i < num_spawns; ++i) {
            int32_t spawn_x_net;
            memcpy(&spawn_x_net, buffer + offset, sizeof(spawn_x_net));
            spawn_x_net = ntohl(spawn_x_net);
            offset += sizeof(spawn_x_net);

            int32_t spawn_y_net;
            memcpy(&spawn_y_net, buffer + offset, sizeof(spawn_y_net));
            spawn_y_net = ntohl(spawn_y_net);
            offset += sizeof(spawn_y_net);

            spawns.emplace_back(spawn_x_net, spawn_y_net);
        }
    }


    int32_t get_x() const { return x; }

    int32_t get_y() const { return y; }

    int32_t get_width() const { return site_width; }

    int32_t get_height() const { return site_height; }

    const std::vector<std::pair<int32_t, int32_t>>& get_spawns() const { return spawns; }
};

#endif
