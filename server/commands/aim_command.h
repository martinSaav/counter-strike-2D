#ifndef AIM_COMMAND_H
#define AIM_COMMAND_H
#include "player_command.h"
#include "player_credentials.h"
#include "position.h"


// Actualiza hacia donde apunta el jugador aunque no se mueva ni dispare, para que el resto lo
// vea girar.
class AimCommand: public PlayerCommand {
    PlayerCredentials credentials;
    Position aim_pos;

public:
    AimCommand(PlayerCredentials credentials, Position aim_pos);
    void process_command(Match* match) override;
};


#endif  // AIM_COMMAND_H
