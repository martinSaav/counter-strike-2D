//
// Created by matias on 12/06/25.
//

#ifndef BUY_WEAPON_COMMAND_H
#define BUY_WEAPON_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"
#include "weapon.h"


class BuyWeaponCommand: public PlayerCommand {
    PlayerCredentials credentials;
    Weapon weapon;

public:
    BuyWeaponCommand(PlayerCredentials credentials, Weapon weapon);
    void process_command(Match* match) override;
};


#endif  // BUY_WEAPON_COMMAND_H
