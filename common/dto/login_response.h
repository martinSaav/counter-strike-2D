#ifndef JOIN_TEAM_RESPONSE_H
#define JOIN_TEAM_RESPONSE_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class LoginResponse: public Message {
private:
    MessageType message_type = MessageType::LoginResponse;
    bool success;

public:
    explicit LoginResponse(bool success): success(std::move(success)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(1));
        memcpy(buffer + 1, &length, sizeof(length));
        buffer[3] = static_cast<uint8_t>(success);
    }

    static LoginResponse deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        const bool success_deserialized = static_cast<bool>(buffer[3]);
        return LoginResponse(success_deserialized);
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override { return HEADER_SIZE + sizeof(success); }

    const bool& get_success() const { return success; }
};
#endif
