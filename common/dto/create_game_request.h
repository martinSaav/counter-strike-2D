#ifndef CREATE_GAME_REQUEST_H
#define CREATE_GAME_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class CreateGameRequest: public Message {
private:
    MessageType message_type = MessageType::CreateGameRequest;
    std::string game_name;
    std::string map_name;

public:
    explicit CreateGameRequest(std::string game_name, std::string map_name):
            game_name(std::move(game_name)), map_name(std::move(map_name)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        size_t offset = HEADER_SIZE;
        uint16_t game_name_length = htons(static_cast<uint16_t>(game_name.size()));
        memcpy(buffer + offset, &game_name_length, sizeof(game_name_length));
        offset += sizeof(game_name_length);
        memcpy(buffer + offset, game_name.data(), game_name.size());
        offset += game_name.size();
        uint16_t map_name_length = htons(static_cast<uint16_t>(map_name.size()));
        memcpy(buffer + offset, &map_name_length, sizeof(map_name_length));
        offset += sizeof(map_name_length);
        memcpy(buffer + offset, map_name.data(), map_name.size());
        offset += map_name.size();

        uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
        memcpy(buffer + 1, &payload_length, sizeof(payload_length));
    }

    static CreateGameRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        size_t offset = HEADER_SIZE;

        uint16_t game_name_length;
        memcpy(&game_name_length, buffer + offset, sizeof(game_name_length));
        game_name_length = ntohs(game_name_length);
        offset += sizeof(game_name_length);

        std::string game_name(reinterpret_cast<const char*>(buffer + offset), game_name_length);
        offset += game_name_length;

        uint16_t map_name_length;
        memcpy(&map_name_length, buffer + offset, sizeof(map_name_length));
        map_name_length = ntohs(map_name_length);
        offset += sizeof(map_name_length);

        std::string map_name(reinterpret_cast<const char*>(buffer + offset), map_name_length);
        // offset += map_name_length;

        return CreateGameRequest(std::move(game_name), std::move(map_name));
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE; // header
        size += sizeof(uint16_t) + game_name.size(); // game name
        size += sizeof(uint16_t) + map_name.size(); // map name
        return size;
    }
    
    const std::string& get_game_name() const { return game_name; }

    const std::string& get_map_name() const { return map_name; }
};
#endif
