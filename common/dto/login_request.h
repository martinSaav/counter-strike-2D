#ifndef LOGIN_REQUEST_H
#define LOGIN_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class LoginRequest: public Message {
private:
    MessageType message_type = MessageType::LoginRequest;
    std::string username;

public:
    explicit LoginRequest(std::string username): username(std::move(username)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(username.size()));
        memcpy(buffer + 1, &length, sizeof(length));
        memcpy(buffer + 3, username.data(), username.size());
    }

    static LoginRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        const std::string username_deserialized(reinterpret_cast<const char*>(buffer + 3), length);
        return LoginRequest(username_deserialized);
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override { return HEADER_SIZE + username.size(); }

    const std::string& get_username() const { return username; }
};
#endif
