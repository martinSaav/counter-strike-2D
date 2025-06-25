#ifndef STRUCTURE_INFO_H
#define STRUCTURE_INFO_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>

#include <arpa/inet.h>

class StructureInfo {
private:
    int32_t height;
    int32_t width;
    int32_t x;
    int32_t y;

public:
    StructureInfo(int32_t height, int32_t width, int32_t x, int32_t y):
            height(height), width(width), x(x), y(y) {}

    void serialize(uint8_t* buffer) const {
        size_t offset = 0;

        int32_t height_net = htonl(height);
        memcpy(buffer + offset, &height_net, sizeof(height_net));
        offset += sizeof(height_net);

        int32_t width_net = htonl(width);
        memcpy(buffer + offset, &width_net, sizeof(width_net));
        offset += sizeof(width_net);

        int32_t x_net = htonl(x);
        memcpy(buffer + offset, &x_net, sizeof(x_net));
        offset += sizeof(x_net);

        int32_t y_net = htonl(y);
        memcpy(buffer + offset, &y_net, sizeof(y_net));
    }

    static StructureInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }

        size_t offset = 0;

        int32_t height;
        memcpy(&height, buffer + offset, sizeof(height));
        height = ntohl(height);
        offset += sizeof(height);

        int32_t width;
        memcpy(&width, buffer + offset, sizeof(width));
        width = ntohl(width);
        offset += sizeof(width);

        int32_t x;
        memcpy(&x, buffer + offset, sizeof(x));
        x = ntohl(x);
        offset += sizeof(x);

        int32_t y;
        memcpy(&y, buffer + offset, sizeof(y));
        y = ntohl(y);

        return StructureInfo(height, width, x, y);
    }

    size_t serialized_size() const { return sizeof(int32_t) * 4; }

    int32_t get_height() const { return height; }

    int32_t get_width() const { return width; }

    int32_t get_x() const { return x; }

    int32_t get_y() const { return y; }
};

#endif
