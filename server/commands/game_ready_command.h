//
// Created by matias on 12/06/25.
//

#ifndef GAME_READY_COMMAND_H
#define GAME_READY_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


class GameReadyCommand: public PlayerCommand {
    PlayerCredentials credentials;

public:
    explicit GameReadyCommand(PlayerCredentials credentials);
    void process_command(Match* match) override;
};


#endif  // GAME_READY_COMMAND_H
