//
// Created by matias on 16/05/25.
//

#ifndef PLAYERCOMMAND_H
#define PLAYERCOMMAND_H

#include <set>
#include <stdexcept>

#include "events.h"
#include "player_credentials.h"

struct InvalidCommandType: public std::runtime_error {
    InvalidCommandType(): std::runtime_error("Invalid Command Type") {}
};

class PlayerCommand {
    friend class Match;
    static std::set<CommandTypes> get_player_commands_set();
    PlayerCommand();

public:
    const PlayerCredentials credentials;
    const CommandTypes command_type;

    PlayerCommand(const PlayerCredentials& credentials, const CommandTypes type);
};


#endif  // PLAYERCOMMAND_H
