#ifndef LOBBY_H
#define LOBBY_H
#include <map>
#include <mutex>
#include <string>

#include "match.h"

class Lobby {
    std::map<std::string, Match> matches;
    std::mutex mutex;
};

#endif  // LOBBY_H
