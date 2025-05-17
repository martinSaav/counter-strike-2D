#ifndef MAP_NAMES_RESPONSE_H
#define MAP_NAMES_RESPONSE_H

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include <arpa/inet.h>

#include "../message.h"
#include "../message_type.h"

class MapNamesResponse: public Message {
private:
    MessageType message_type = MessageType::MapNamesRequest;    
    std::string map_names;

public:
    explicit MapNamesResponse(std::string map_names): map_names(std::move(map_names)) {}

    void serialize(uint8_t* buffer) const {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = static_cast<uint16_t>(map_names.size());
        std::memcpy(buffer + 1, &length, sizeof(length));
        std::memcpy(buffer + 3, map_names.data(), length);
    }

    size_t serialized_size() const { return 3 + map_names.size(); }

    MessageType type() const override { return message_type; }

    static MapNamesResponse deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        std::memcpy(&length, buffer, sizeof(length));
        const std::string map_names_deserialized(
                reinterpret_cast<const char*>(buffer + sizeof(length)), length);
        return MapNamesResponse(map_names_deserialized);
    }
};

#endif