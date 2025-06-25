//
// Created by matias on 13/06/25.
//

#ifndef DEFUSE_BOMB_COMMAND_H
#define DEFUSE_BOMB_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


class DefuseBombCommand: public PlayerCommand {
    PlayerCredentials credentials;

public:
    explicit DefuseBombCommand(PlayerCredentials credentials);
    void process_command(Match* match) override;
};


#endif  // DEFUSE_BOMB_COMMAND_H
