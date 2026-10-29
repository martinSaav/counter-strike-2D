#include <cstring>
#include <limits>
#include <string>

#include "byte_reader.h"
#include "byte_writer.h"

uint16_t ByteWriter::checked_u16(std::size_t size, const char* what) {
    if (size > std::numeric_limits<uint16_t>::max()) {
        throw ProtocolError(std::string(what) + " demasiado larga para el protocolo");
    }
    return static_cast<uint16_t>(size);
}

void ByteWriter::f32(float value) {
    uint32_t bits;
    static_assert(sizeof(bits) == sizeof(value));
    std::memcpy(&bits, &value, sizeof(bits));
    u32(bits);
}

void ByteWriter::string(const std::string& value) {
    u16(checked_u16(value.size(), "texto"));
    bytes.insert(bytes.end(), value.begin(), value.end());
}

void ByteReader::require(std::size_t count, const char* what) const {
    if (count > size - offset) {
        throw ProtocolError(std::string("mensaje truncado leyendo ") + what);
    }
}

uint8_t ByteReader::u8() {
    require(1, "un byte");
    return data[offset++];
}

uint16_t ByteReader::u16() {
    require(2, "un uint16");
    const uint16_t value = static_cast<uint16_t>((data[offset] << 8) | data[offset + 1]);
    offset += 2;
    return value;
}

uint32_t ByteReader::u32() {
    const uint32_t high = u16();
    const uint32_t low = u16();
    return (high << 16) | low;
}

float ByteReader::f32() {
    const uint32_t bits = u32();
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

std::string ByteReader::string() {
    const uint16_t length = u16();
    require(length, "un texto");
    std::string value(reinterpret_cast<const char*>(data + offset), length);
    offset += length;
    return value;
}
