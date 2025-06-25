//
// Created by matias on 12/06/25.
//

#ifndef LEAVE_MATCH_COMMAND_H
#define LEAVE_MATCH_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


class LeaveMatchCommand: public PlayerCommand {
    PlayerCredentials credentials;

public:
    explicit LeaveMatchCommand(PlayerCredentials credentials);
    void process_command(Match* match) override;
};


#endif  // LEAVE_MATCH_COMMAND_H
