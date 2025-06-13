//
// Created by matias on 13/06/25.
//

#include "equip_weapon_command.h"

#include "match.h"

EquipWeaponCommand::EquipWeaponCommand(const PlayerCredentials credentials):
        credentials(credentials) {}


void EquipWeaponCommand::process_command(Match* match) {
    RequestProcessor::process_pick_weapon_request(credentials, match);
}
