//
// Created by matias on 13/06/25.
//

#include "reload_weapon_command.h"

#include "match.h"

ReloadWeaponCommand::ReloadWeaponCommand(const PlayerCredentials credentials):
        credentials(credentials) {}


void ReloadWeaponCommand::process_command(Match* match) {
    RequestProcessor::process_reload_request(credentials, match);
}
