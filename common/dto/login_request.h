#ifndef LOGIN_REQUEST_H
#define LOGIN_REQUEST_H

#include "../message_type.h"
#include "../message.h"
#include <cstring>
#include <string>
#include <utility>
#include <netinet/in.h>
#include <stdexcept>

class LoginRequest: public Message {
private:
    MessageType message_type = MessageType::LoginRequest;
    std::string username;
public:
    explicit LoginRequest(std::string username)
        : username(std::move(username)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(username.size()));
        memcpy(buffer + 1, &length, sizeof(length));
        memcpy(buffer + 3, username.data(), username.size());
    }

    size_t serialized_size() const override {
        return 3 + username.size() + username.size();
    }

    const std::string& get_username() const {
        return username;
    }

    static LoginRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error(
                    "");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        size_t expected_size = 3 + static_cast<size_t>(length);
        if (size < expected_size) {
            throw std::runtime_error(
                    "");
        }
        const std::string username_deserialized(
                reinterpret_cast<const char*>(buffer + 3), length);
        return LoginRequest(username_deserialized);
    }

};
#endif
