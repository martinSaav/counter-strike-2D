//
// Created by matias on 12/06/25.
//

#ifndef SHOOT_COMMAND_H
#define SHOOT_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"
#include "position.h"


class ShootCommand: public PlayerCommand {
    PlayerCredentials credentials;
    Position aim_pos;

public:
    ShootCommand(PlayerCredentials credentials, Position aim_pos);
    void process_command(Match* match) override;
};


#endif  // SHOOT_COMMAND_H
