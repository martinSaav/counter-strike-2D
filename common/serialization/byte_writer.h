#ifndef BYTE_WRITER_H
#define BYTE_WRITER_H

#include <cstdint>
#include <string>
#include <vector>

#include "protocol_error.h"

// Arma el payload de un mensaje. Todos los enteros van en orden de red (big-endian) y los float
// como sus 32 bits en orden de red, asi el formato no depende de la maquina.
class ByteWriter {
    std::vector<uint8_t> bytes;

public:
    void u8(uint8_t value) { bytes.push_back(value); }

    void boolean(bool value) { u8(value ? 1 : 0); }

    template <typename Enum>
    void enum_u8(Enum value) {
        u8(static_cast<uint8_t>(value));
    }

    void u16(uint16_t value) {
        u8(static_cast<uint8_t>(value >> 8));
        u8(static_cast<uint8_t>(value));
    }

    void u32(uint32_t value) {
        u16(static_cast<uint16_t>(value >> 16));
        u16(static_cast<uint16_t>(value));
    }

    void i32(int32_t value) { u32(static_cast<uint32_t>(value)); }

    void f32(float value);

    // Largo (uint16) y los bytes del texto
    void string(const std::string& value);

    // Cantidad de elementos (uint16) y cada elemento escrito con write_item(writer, elemento)
    template <typename Container, typename WriteItem>
    void list(const Container& items, WriteItem write_item) {
        u16(checked_u16(items.size(), "lista"));
        for (const auto& item: items) {
            write_item(*this, item);
        }
    }

    // Cantidad de elementos como uint32 (la configuracion del mapa la manda asi)
    template <typename Container, typename WriteItem>
    void list_u32(const Container& items, WriteItem write_item) {
        u32(static_cast<uint32_t>(items.size()));
        for (const auto& item: items) {
            write_item(*this, item);
        }
    }

    [[nodiscard]] const std::vector<uint8_t>& data() const { return bytes; }

    static uint16_t checked_u16(std::size_t size, const char* what);
};

#endif  // BYTE_WRITER_H
