#ifndef MAP_NAMES_RESPONSE_H
#define MAP_NAMES_RESPONSE_H

#include <list>
#include <string>
#include <utility>

#include "../message.h"

class MapNamesResponse: public MessageOf<MessageType::MapNamesResponse> {
private:
    std::list<std::string> map_names;

public:
    explicit MapNamesResponse(std::list<std::string> map_names): map_names(std::move(map_names)) {}

    void write_payload(ByteWriter& writer) const override {
        writer.list(map_names, [](ByteWriter& w, const std::string& name) { w.string(name); });
    }

    static MapNamesResponse read_payload(ByteReader& reader) {
        return MapNamesResponse(
                reader.list<std::list<std::string>>([](ByteReader& r) { return r.string(); }));
    }

    const std::list<std::string>& get_mapGames() const { return map_names; }
};

#endif
