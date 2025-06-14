//
// Created by matias on 12/06/25.
//

#ifndef MOVEMENT_COMMAND_H
#define MOVEMENT_COMMAND_H
#include <set>

#include "command_type.h"
#include "player_command.h"
#include "player_credentials.h"
#include "position.h"


class MovementCommand: public PlayerCommand {
    PlayerCredentials credentials;
    CommandType command;
    Position aim_pos;
    static std::set<CommandType> get_valid_commands();

public:
    MovementCommand(PlayerCredentials credentials, CommandType command, Position aim_pos);
    void process_command(Match* match) override;
};


#endif  // MOVEMENT_COMMAND_H
