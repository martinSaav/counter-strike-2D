#include <limits>
#include <string>
#include <vector>

#include "common/serialization/byte_reader.h"
#include "common/serialization/byte_writer.h"
#include "gtest/gtest.h"

TEST(ByteIoTest, WritesIntegersInNetworkOrder) {
    ByteWriter writer;
    writer.u8(0x01);
    writer.u16(0x0203);
    writer.i32(0x04050607);
    writer.boolean(true);

    const std::vector<uint8_t> expected = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x01};
    EXPECT_EQ(writer.data(), expected);
}

TEST(ByteIoTest, WritesFloatBitsInNetworkOrder) {
    ByteWriter writer;
    writer.f32(1.0f);  // IEEE 754: 0x3F800000

    const std::vector<uint8_t> expected = {0x3F, 0x80, 0x00, 0x00};
    EXPECT_EQ(writer.data(), expected);
}

TEST(ByteIoTest, WritesStringWithLength) {
    ByteWriter writer;
    writer.string("ab");

    const std::vector<uint8_t> expected = {0x00, 0x02, 'a', 'b'};
    EXPECT_EQ(writer.data(), expected);
}

TEST(ByteIoTest, RoundTripsEveryType) {
    ByteWriter writer;
    writer.u8(200);
    writer.u16(65000);
    writer.i32(-123456);
    writer.f32(-2.5f);
    writer.boolean(false);
    writer.string("dust");
    writer.list(std::vector<uint16_t>{1, 2, 3}, [](ByteWriter& w, uint16_t v) { w.u16(v); });
    writer.list_u32(std::vector<std::string>{"a", "bc"},
                    [](ByteWriter& w, const std::string& v) { w.string(v); });

    ByteReader reader(writer.data());
    EXPECT_EQ(reader.u8(), 200);
    EXPECT_EQ(reader.u16(), 65000);
    EXPECT_EQ(reader.i32(), -123456);
    EXPECT_FLOAT_EQ(reader.f32(), -2.5f);
    EXPECT_FALSE(reader.boolean());
    EXPECT_EQ(reader.string(), "dust");
    EXPECT_EQ((reader.list<std::vector<uint16_t>>([](ByteReader& r) { return r.u16(); })),
              (std::vector<uint16_t>{1, 2, 3}));
    EXPECT_EQ((reader.list_u32<std::vector<std::string>>([](ByteReader& r) { return r.string(); })),
              (std::vector<std::string>{"a", "bc"}));
    EXPECT_EQ(reader.remaining(), 0u);
}

TEST(ByteIoTest, TruncatedReadThrows) {
    const std::vector<uint8_t> bytes = {0x01};
    ByteReader reader(bytes);
    EXPECT_THROW(reader.u16(), ProtocolError);
}

TEST(ByteIoTest, StringLongerThanPayloadThrows) {
    // Dice que el texto tiene 10 bytes pero solo vienen 2
    const std::vector<uint8_t> bytes = {0x00, 0x0A, 'a', 'b'};
    ByteReader reader(bytes);
    EXPECT_THROW(reader.string(), ProtocolError);
}

TEST(ByteIoTest, HugeListCountThrows) {
    // Anuncia 4 mil millones de elementos y no manda ninguno
    ByteWriter writer;
    writer.u32(0xFFFFFFFF);
    ByteReader reader(writer.data());
    EXPECT_THROW(reader.list_u32<std::vector<uint8_t>>([](ByteReader& r) { return r.u8(); }),
                 ProtocolError);
}

TEST(ByteIoTest, StringTooLongForProtocolThrows) {
    ByteWriter writer;
    const std::string huge(std::numeric_limits<uint16_t>::max() + 1, 'x');
    EXPECT_THROW(writer.string(huge), ProtocolError);
}
