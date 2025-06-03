#ifndef SELECT_SKIN_REQUEST_H
#define SELECT_SKIN_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"

class SelectSkinRequest: public Message {
private:
    MessageType message_type = MessageType::SelectSkinRequest;
    std::string skin;

public:
    explicit SelectSkinRequest(std::string skin): skin(std::move(skin)) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(static_cast<uint16_t>(skin.size()));
        memcpy(buffer + 1, &length, sizeof(length));
        memcpy(buffer + 3, skin.data(), skin.size());
    }

    size_t serialized_size() const override { return 3 + skin.size(); }

    const std::string& get_skin() const { return skin; }


    static SelectSkinRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        size_t expected_size = 3 + static_cast<size_t>(length);
        if (size < expected_size) {
            throw std::runtime_error("");
        }
        const std::string skin_deserialized(reinterpret_cast<const char*>(buffer + 3), length);
        return SelectSkinRequest(skin_deserialized);
    }

    MessageType type() const override { return this->message_type; }
};
#endif
