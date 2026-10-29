#ifndef JOIN_GAME_REQUEST_H
#define JOIN_GAME_REQUEST_H

#include <string>
#include <utility>

#include "../message.h"

class JoinGameRequest: public MessageOf<MessageType::JoinGameRequest> {
private:
    std::string game_name;

public:
    explicit JoinGameRequest(std::string game_name): game_name(std::move(game_name)) {}

    void write_payload(ByteWriter& writer) const override { writer.string(game_name); }

    static JoinGameRequest read_payload(ByteReader& reader) {
        return JoinGameRequest(reader.string());
    }

    const std::string& get_game_name() const { return game_name; }
};
#endif
