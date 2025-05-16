//
// Created by matias on 16/05/25.
//

#ifndef PLAYERCOMMAND_H
#define PLAYERCOMMAND_H

#include <set>

#include "events.h"
#include "player_credentials.h"


class PlayerCommand {
    static std::set<CommandTypes> get_player_commands_set();

public:
    const PlayerCredentials credentials;
    const CommandTypes type;
    PlayerCommand(const PlayerCredentials& credentials, const CommandTypes type);
};


#endif  // PLAYERCOMMAND_H
