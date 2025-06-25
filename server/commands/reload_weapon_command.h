//
// Created by matias on 13/06/25.
//

#ifndef RELOAD_WEAPON_COMMAND_H
#define RELOAD_WEAPON_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


class ReloadWeaponCommand: public PlayerCommand {
    PlayerCredentials credentials;

public:
    explicit ReloadWeaponCommand(PlayerCredentials credentials);
    void process_command(Match* match) override;
};


#endif  // RELOAD_WEAPON_COMMAND_H
