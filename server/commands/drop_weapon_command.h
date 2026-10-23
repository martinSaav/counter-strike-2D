#ifndef DROP_WEAPON_COMMAND_H
#define DROP_WEAPON_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"


class DropWeaponCommand: public PlayerCommand {
    PlayerCredentials credentials;

public:
    explicit DropWeaponCommand(PlayerCredentials credentials);
    void process_command(Match* match) override;
};


#endif  // DROP_WEAPON_COMMAND_H
