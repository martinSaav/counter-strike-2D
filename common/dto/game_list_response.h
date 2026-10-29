#ifndef GAME_LIST_RESPONSE_H
#define GAME_LIST_RESPONSE_H

#include <list>
#include <utility>

#include "../game_info.h"
#include "../message.h"

class GameListResponse: public MessageOf<MessageType::GameListResponse> {
private:
    std::list<GameInfo> games;

public:
    explicit GameListResponse(std::list<GameInfo> games): games(std::move(games)) {}

    void write_payload(ByteWriter& writer) const override {
        writer.list(games, [](ByteWriter& w, const GameInfo& game) { game.write(w); });
    }

    static GameListResponse read_payload(ByteReader& reader) {
        return GameListResponse(reader.list<std::list<GameInfo>>(GameInfo::read));
    }

    const std::list<GameInfo>& get_games() const { return games; }
};

#endif
