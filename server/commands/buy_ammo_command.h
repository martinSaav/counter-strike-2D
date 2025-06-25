//
// Created by matias on 20/06/25.
//

#ifndef BUY_AMMO_COMMAND_H
#define BUY_AMMO_COMMAND_H
#include "gun/gun_type.h"

#include "player_command.h"
#include "player_credentials.h"


class BuyAmmoCommand: public PlayerCommand {
    PlayerCredentials credentials;
    GunType weapon_type;

public:
    BuyAmmoCommand(PlayerCredentials credentials, GunType weapon_type);
    void process_command(Match* match) override;
};


#endif  // BUY_AMMO_COMMAND_H
