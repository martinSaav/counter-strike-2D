//
// Created by matias on 12/06/25.
//

#ifndef CHANGE_SKIN_COMMAND_H
#define CHANGE_SKIN_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"
#include "player_skin.h"


class ChangeSkinCommand: public PlayerCommand {
    PlayerCredentials credentials;
    PlayerSkin new_skin;

public:
    ChangeSkinCommand(PlayerCredentials credentials, PlayerSkin new_skin);
    void process_command(Match* match) override;
};


#endif  // CHANGE_SKIN_COMMAND_H
