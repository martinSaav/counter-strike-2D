#ifndef RECT_INFO_H
#define RECT_INFO_H

#include <cstdint>

#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"

// Un rectangulo del mapa: una estructura (pared, caja) o una zona de plantado.
class RectInfo {
private:
    int32_t height;
    int32_t width;
    int32_t x;
    int32_t y;

public:
    RectInfo(int32_t height, int32_t width, int32_t x, int32_t y):
            height(height), width(width), x(x), y(y) {}

    void write(ByteWriter& writer) const {
        writer.i32(height);
        writer.i32(width);
        writer.i32(x);
        writer.i32(y);
    }

    static RectInfo read(ByteReader& reader) {
        const int32_t height = reader.i32();
        const int32_t width = reader.i32();
        const int32_t x = reader.i32();
        const int32_t y = reader.i32();
        return RectInfo(height, width, x, y);
    }

    int32_t get_height() const { return height; }

    int32_t get_width() const { return width; }

    int32_t get_x() const { return x; }

    int32_t get_y() const { return y; }
};

using StructureInfo = RectInfo;
using BombSiteInfo = RectInfo;

#endif
