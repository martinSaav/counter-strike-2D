#ifndef LOBBY_H
#define LOBBY_H
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "game_config.h"
#include "game_identification.h"
#include "match.h"
#include "match_dto.h"

struct MatchNotFound: public std::runtime_error {
    MatchNotFound(): std::runtime_error("Match not found") {}
};

struct MatchAlreadyExists: public std::runtime_error {
    MatchAlreadyExists(): std::runtime_error("Match already exists") {}
};

class Lobby {
    std::map<std::string, std::unique_ptr<Match>> matches;
    GameConfig game_config;
    std::mutex mutex;

public:
    explicit Lobby(GameConfig&& game_config): game_config(std::move(game_config)) {}
    GameIdentification create_match(const std::string& match_name, const std::string& player_name);
    GameIdentification join_match(const std::string& match_name, const std::string& player_name);
    std::vector<MatchDTO> list_matches();
    void remove_finished_matches();
    void kill_all_matches();
};

#endif  // LOBBY_H
