//
// Created by matias on 16/05/25.
//

#ifndef PLAYERCOMMAND_H
#define PLAYERCOMMAND_H

#include <set>
#include <stdexcept>

#include "common/action.h"

#include "events.h"
#include "player_credentials.h"

struct InvalidCommandType: public std::runtime_error {
    InvalidCommandType(): std::runtime_error("Invalid Command Type") {}
};

class PlayerCommand {
    friend class Match;
    PlayerCommand();

public:
    PlayerCredentials credentials;
    Action command_type;

    PlayerCommand(const PlayerCredentials& credentials, Action type);
};


#endif  // PLAYERCOMMAND_H
