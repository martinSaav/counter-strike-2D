#ifndef BYTE_READER_H
#define BYTE_READER_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "protocol_error.h"

// Lee el payload de un mensaje con el mismo formato que ByteWriter. Controla los limites: si un
// campo o un largo no entra en lo que queda, lanza ProtocolError en vez de leer fuera del buffer.
class ByteReader {
    const uint8_t* data;
    std::size_t size;
    std::size_t offset = 0;

    void require(std::size_t count, const char* what) const;

public:
    ByteReader(const uint8_t* data, std::size_t size): data(data), size(size) {}
    explicit ByteReader(const std::vector<uint8_t>& bytes):
            data(bytes.data()), size(bytes.size()) {}

    uint8_t u8();
    bool boolean() { return u8() != 0; }

    template <typename Enum>
    Enum enum_u8() {
        return static_cast<Enum>(u8());
    }

    uint16_t u16();
    uint32_t u32();
    int32_t i32() { return static_cast<int32_t>(u32()); }
    float f32();
    std::string string();

    // Cantidad (uint16) y cada elemento leido con read_item(reader)
    template <typename Container, typename ReadItem>
    Container list(ReadItem read_item) {
        Container items;
        const uint16_t count = u16();
        for (uint16_t i = 0; i < count; ++i) {
            items.push_back(read_item(*this));
        }
        return items;
    }

    // Cantidad como uint32 (ver ByteWriter::list_u32). Una cantidad absurda no hace dano: cada
    // elemento lee al menos un byte, asi que se corta con ProtocolError al terminarse el payload.
    template <typename Container, typename ReadItem>
    Container list_u32(ReadItem read_item) {
        Container items;
        const uint32_t count = u32();
        for (uint32_t i = 0; i < count; ++i) {
            items.push_back(read_item(*this));
        }
        return items;
    }

    [[nodiscard]] std::size_t remaining() const { return size - offset; }
};

#endif  // BYTE_READER_H
