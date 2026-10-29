//
// Created by matias on 13/06/25.
//

#include "switch_weapon_command.h"

#include "match.h"

SwitchWeaponCommand::SwitchWeaponCommand(PlayerCredentials credentials, GunType weapon_type):
        credentials(credentials), weapon_type(weapon_type) {}


void SwitchWeaponCommand::process_command(Match* match) {
    match->process_switch_weapon_request(credentials, weapon_type);
}
