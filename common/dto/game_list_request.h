#ifndef GAME_LIST_REQUEST_H
#define GAME_LIST_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class GameListRequest: public Message {
private:
    MessageType message_type = MessageType::GameListRequest;

public:
    GameListRequest() = default;

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        buffer[1] = 0;
        buffer[2] = 0;
    }

    size_t serialized_size() const override { return 3; }

    static GameListRequest deserialize([[maybe_unused]] const uint8_t* buffer, size_t size) {
        if (size < 1) {
            throw std::runtime_error("");
        }
        return GameListRequest();
    }

    MessageType type() const override { return this->message_type; }
};
#endif
