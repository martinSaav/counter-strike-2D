#ifndef MAP_NAMES_REQUEST_H
#define MAP_NAMES_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class MapNamesRequest: public Message {
private:
    MessageType message_type = MessageType::MapNamesRequest;

public:
    MapNamesRequest() = default;

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        buffer[1] = 0;
        buffer[2] = 0;
    }

    static MapNamesRequest deserialize([[maybe_unused]] const uint8_t* buffer, size_t size) {
        if (size < 1) {
            throw std::runtime_error("");
        }
        return MapNamesRequest();
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override { return HEADER_SIZE; }
};
#endif
