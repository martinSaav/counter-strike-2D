//
// Created by matias on 13/06/25.
//

#ifndef SWITCH_WEAPON_COMMAND_H
#define SWITCH_WEAPON_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


enum class GunType;

class SwitchWeaponCommand: public PlayerCommand {
    PlayerCredentials credentials;
    GunType weapon_type;

public:
    SwitchWeaponCommand(PlayerCredentials credentials, GunType weapon_type);
    void process_command(Match* match) override;
};


#endif  // SWITCH_WEAPON_COMMAND_H
