#ifndef LOBBY_H
#define LOBBY_H
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "common/catedra/queue.h"

#include "game_identification.h"
#include "match.h"

struct MatchNotFound: public std::runtime_error {
    MatchNotFound(): std::runtime_error("Match not found") {}
};

struct MatchAlreadyExists: public std::runtime_error {
    MatchAlreadyExists(): std::runtime_error("Match already exists") {}
};

class Lobby {
    std::map<std::string, std::unique_ptr<Match>> matches;
    std::mutex mutex;

public:
    Lobby() = default;
    GameIdentification create_match(const std::string& match_name, const std::string& player_name);
    GameIdentification join_match(const std::string& match_name, const std::string& player_name);
};

#endif  // LOBBY_H
