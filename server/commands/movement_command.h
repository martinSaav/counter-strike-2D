//
// Created by matias on 12/06/25.
//

#ifndef MOVEMENT_COMMAND_H
#define MOVEMENT_COMMAND_H
#include <set>

#include "movement_type.h"
#include "player_command.h"
#include "player_credentials.h"
#include "position.h"


class MovementCommand: public PlayerCommand {
    PlayerCredentials credentials;
    MovementType command;
    Position aim_pos;

public:
    MovementCommand(PlayerCredentials credentials, MovementType command, Position aim_pos);
    void process_command(Match* match) override;
};


#endif  // MOVEMENT_COMMAND_H
