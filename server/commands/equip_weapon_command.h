//
// Created by matias on 13/06/25.
//

#ifndef EQUIP_WEAPON_COMMAND_H
#define EQUIP_WEAPON_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


class EquipWeaponCommand: public PlayerCommand {
    PlayerCredentials credentials;

public:
    explicit EquipWeaponCommand(PlayerCredentials credentials);
    void process_command(Match* match) override;
};


#endif  // EQUIP_WEAPON_COMMAND_H
