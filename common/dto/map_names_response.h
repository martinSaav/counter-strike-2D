#ifndef MAP_NAMES_RESPONSE_H
#define MAP_NAMES_RESPONSE_H

#include <cstring>
#include <iostream>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>

#include <arpa/inet.h>

#include "../message.h"
#include "../message_type.h"

class MapNamesResponse: public Message {
private:
    MessageType message_type = MessageType::MapNamesResponse;
    std::list<std::string> map_names;

public:
    explicit MapNamesResponse(std::list<std::string> map_names): map_names(std::move(map_names)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        size_t offset = HEADER_SIZE;
        for (const auto& map_name: map_names) {
            uint16_t map_name_length = htons(static_cast<uint16_t>(map_name.size()));
            std::memcpy(buffer + offset, &map_name_length, sizeof(map_name_length));
            offset += sizeof(map_name_length);
            std::memcpy(buffer + offset, map_name.data(), map_name.size());
            offset += map_name.size();
        }
        uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
        std::memcpy(buffer + 1, &payload_length, sizeof(payload_length));
    }

    static MapNamesResponse deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        std::memcpy(&length, buffer + 1, sizeof(length));

        std::list<std::string> map_names_deserialized;
        size_t offset = 3;
        while (offset < static_cast<size_t>(length + 3)) {
            uint16_t map_name_length;
            std::memcpy(&map_name_length, buffer + offset, sizeof(map_name_length));
            map_name_length = ntohs(map_name_length);
            offset += sizeof(map_name_length);

            const std::string map_name(reinterpret_cast<const char*>(buffer + offset),
                                       map_name_length);
            map_names_deserialized.push_back(map_name);
            offset += map_name_length;
        }
        return MapNamesResponse(map_names_deserialized);
    }

    MessageType type() const override { return message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE;
        size += std::accumulate(map_names.begin(), map_names.end(), 0,
                                [](int sum, const std::string& str) { return sum + str.size(); });
        size += map_names.size() * sizeof(uint16_t);
        return size;
    }

    const std::list<std::string>& get_mapGames() const { return map_names; }
};

#endif
