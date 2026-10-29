#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <string>

#include "serialization/byte_reader.h"
#include "serialization/byte_writer.h"

struct GameInfo {
    std::string name;
    std::string map_name;
    uint8_t current_players;
    uint8_t max_players;

    void write(ByteWriter& writer) const {
        writer.string(name);
        writer.string(map_name);
        writer.u8(current_players);
        writer.u8(max_players);
    }

    static GameInfo read(ByteReader& reader) {
        GameInfo game;
        game.name = reader.string();
        game.map_name = reader.string();
        game.current_players = reader.u8();
        game.max_players = reader.u8();
        return game;
    }
};

#endif
