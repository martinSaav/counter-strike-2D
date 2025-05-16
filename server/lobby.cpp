#include "lobby.h"
GameIdentification Lobby::create_match(const std::string& match_name,
                                       const std::string& player_name) {
    std::lock_guard<std::mutex> guard(mutex);
    if (matches.contains(match_name)) {
        throw MatchAlreadyExists();
    }

    matches[match_name] = std::make_unique<Match>();
    return matches[match_name]->join_match(player_name);
}

GameIdentification Lobby::join_match(const std::string& match_name,
                                     const std::string& player_name) {
    std::lock_guard<std::mutex> guard(mutex);
    const auto match = matches.find(match_name);
    if (match == matches.end()) {
        throw MatchNotFound();
    }

    return match->second->join_match(player_name);
}
