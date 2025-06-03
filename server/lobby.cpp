#include "lobby.h"
GameIdentification Lobby::create_match(const std::string& match_name,
                                       const std::string& player_name) {
    std::lock_guard<std::mutex> guard(mutex);
    if (matches.contains(match_name)) {
        throw MatchAlreadyExists();
    }

    matches[match_name] = std::make_unique<Match>();
    const GameIdentification game_id = matches[match_name]->join_match(player_name);
    matches[match_name]->start();
    return game_id;
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

std::vector<MatchDTO> Lobby::list_matches() {
    std::lock_guard<std::mutex> guard(mutex);
    std::vector<MatchDTO> match_dtos;
    for (auto& [name, match]: matches) {
        MatchDTO match_dto(name, match->get_player_count(), match->get_max_player_count());
        match_dtos.push_back(match_dto);
    }
    return match_dtos;
}
