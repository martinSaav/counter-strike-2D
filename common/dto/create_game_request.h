#ifndef CREATE_GAME_REQUEST_H
#define CREATE_GAME_REQUEST_H

#include <string>
#include <utility>

#include "../message.h"

class CreateGameRequest: public MessageOf<MessageType::CreateGameRequest> {
private:
    std::string game_name;
    std::string map_name;

public:
    CreateGameRequest(std::string game_name, std::string map_name):
            game_name(std::move(game_name)), map_name(std::move(map_name)) {}

    void write_payload(ByteWriter& writer) const override {
        writer.string(game_name);
        writer.string(map_name);
    }

    static CreateGameRequest read_payload(ByteReader& reader) {
        std::string game_name = reader.string();
        std::string map_name = reader.string();
        return CreateGameRequest(std::move(game_name), std::move(map_name));
    }

    const std::string& get_game_name() const { return game_name; }

    const std::string& get_map_name() const { return map_name; }
};
#endif
