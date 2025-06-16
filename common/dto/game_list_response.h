#ifndef GAME_LIST_RESPONSE_H
#define GAME_LIST_RESPONSE_H

#include <cstring>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

#include <arpa/inet.h>

#include "../game_info.h"
#include "../message.h"
#include "../message_type.h"

class GameListResponse: public Message {
private:
    MessageType message_type = MessageType::GameListResponse;
    std::list<GameInfo> games;

public:
    explicit GameListResponse(std::list<GameInfo> games): games(std::move(games)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        size_t offset = HEADER_SIZE;
        for (const auto& game: games) {
            uint16_t name_length = htons(static_cast<uint16_t>(game.name.size()));
            std::memcpy(buffer + offset, &name_length, sizeof(name_length));
            offset += sizeof(name_length);
            std::memcpy(buffer + offset, game.name.data(), game.name.size());
            offset += game.name.size();

            uint16_t map_name_length = htons(static_cast<uint16_t>(game.map_name.size()));
            std::memcpy(buffer + offset, &map_name_length, sizeof(map_name_length));
            offset += sizeof(map_name_length);
            std::memcpy(buffer + offset, game.map_name.data(), game.map_name.size());
            offset += game.map_name.size();

            buffer[offset++] = game.current_players;
            buffer[offset++] = game.max_players;
        }
        uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
        std::memcpy(buffer + 1, &payload_length, sizeof(payload_length));
    }

    static GameListResponse deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        std::memcpy(&length, buffer + 1, sizeof(length));

        std::list<GameInfo> games_deserialized;
        size_t offset = 3;
        while (offset < static_cast<size_t>(length + 3)) {
            uint16_t name_length;
            std::memcpy(&name_length, buffer + offset, sizeof(name_length));
            name_length = ntohs(name_length);
            offset += sizeof(name_length);
            const std::string name(reinterpret_cast<const char*>(buffer + offset), name_length);
            offset += name_length;
            uint16_t map_name_length;
            std::memcpy(&map_name_length, buffer + offset, sizeof(map_name_length));
            map_name_length = ntohs(map_name_length);
            offset += sizeof(map_name_length);
            const std::string map_name(reinterpret_cast<const char*>(buffer + offset),
                                       map_name_length);
            offset += map_name_length;
            uint8_t current_players = buffer[offset++];
            uint8_t max_players = buffer[offset++];
            games_deserialized.emplace_back(GameInfo{name, map_name, current_players, max_players});
        }
        return GameListResponse(games_deserialized);
    }

    MessageType type() const override { return message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE; // header
        for (const auto& game: games) {
            size += sizeof(uint16_t) + game.name.size();
            size += sizeof(uint16_t) + game.map_name.size();
            size += sizeof(uint8_t) * 2;
        }
        return size;
    }

    const std::list<GameInfo>& get_games() const { return games; }
};

#endif
