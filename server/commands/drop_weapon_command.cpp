#include "drop_weapon_command.h"

#include "match.h"

DropWeaponCommand::DropWeaponCommand(const PlayerCredentials credentials):
        credentials(credentials) {}


void DropWeaponCommand::process_command(Match* match) {
    match->process_drop_weapon_request(credentials);
}
