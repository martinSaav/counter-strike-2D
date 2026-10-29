#ifndef SELECT_SKIN_REQUEST_H
#define SELECT_SKIN_REQUEST_H

#include <string>
#include <utility>

#include "../message.h"

class SelectSkinRequest: public MessageOf<MessageType::SelectSkinRequest> {
private:
    std::string skin;

public:
    explicit SelectSkinRequest(std::string skin): skin(std::move(skin)) {}

    void write_payload(ByteWriter& writer) const override { writer.string(skin); }

    static SelectSkinRequest read_payload(ByteReader& reader) {
        return SelectSkinRequest(reader.string());
    }

    const std::string& get_skin() const { return skin; }
};
#endif
