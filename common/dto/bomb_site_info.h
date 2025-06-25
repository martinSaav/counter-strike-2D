#ifndef BOMB_SITE_INFO_H
#define BOMB_SITE_INFO_H

#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>

class BombSiteInfo {
private:
    int32_t bomb_site_height;
    int32_t bomb_site_width;
    int32_t x;
    int32_t y;

public:
    BombSiteInfo(int32_t bomb_site_height, int32_t bomb_site_width, int32_t x, int32_t y):
            bomb_site_height(bomb_site_height), bomb_site_width(bomb_site_width), x(x), y(y) {}

    void serialize(uint8_t* buffer) const {
        size_t offset = 0;

        int32_t height_net = htonl(bomb_site_height);
        memcpy(buffer + offset, &height_net, sizeof(height_net));
        offset += sizeof(height_net);

        int32_t width_net = htonl(bomb_site_width);
        memcpy(buffer + offset, &width_net, sizeof(width_net));
        offset += sizeof(width_net);

        int32_t x_net = htonl(x);
        memcpy(buffer + offset, &x_net, sizeof(x_net));
        offset += sizeof(x_net);

        int32_t y_net = htonl(y);
        memcpy(buffer + offset, &y_net, sizeof(y_net));
    }

    static BombSiteInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }

        size_t offset = 0;
        int32_t bomb_site_height;
        memcpy(&bomb_site_height, buffer + offset, sizeof(bomb_site_height));
        bomb_site_height = ntohl(bomb_site_height);
        offset += sizeof(bomb_site_height);

        int32_t bomb_site_width;
        memcpy(&bomb_site_width, buffer + offset, sizeof(bomb_site_width));
        bomb_site_width = ntohl(bomb_site_width);
        offset += sizeof(bomb_site_width);

        int32_t x;
        memcpy(&x, buffer + offset, sizeof(x));
        x = ntohl(x);
        offset += sizeof(x);

        int32_t y;
        memcpy(&y, buffer + offset, sizeof(y));
        y = ntohl(y);
        offset += sizeof(y);

        return BombSiteInfo(bomb_site_height, bomb_site_width, x, y);
    }

    size_t serialized_size() const {
        return sizeof(int32_t) * 4;  // 4 int32_t values: height, width, x, y
    }

    int32_t get_x() const { return x; }

    int32_t get_y() const { return y; }

    int32_t get_width() const { return bomb_site_width; }

    int32_t get_height() const { return bomb_site_height; }
};

#endif
