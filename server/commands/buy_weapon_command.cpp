//
// Created by matias on 12/06/25.
//

#include "buy_weapon_command.h"

#include "match.h"


BuyWeaponCommand::BuyWeaponCommand(const PlayerCredentials credentials, const Weapon weapon):
        credentials(credentials), weapon(weapon) {}


void BuyWeaponCommand::process_command(Match* match) {
    match->process_buy_weapon_request(credentials, weapon);
}
