//
// Created by matias on 20/06/25.
//

#include "buy_ammo_command.h"

#include "request_processor.h"

BuyAmmoCommand::BuyAmmoCommand(const PlayerCredentials credentials, const GunType weapon_type):
        credentials(credentials), weapon_type(weapon_type) {}


void BuyAmmoCommand::process_command(Match* match) {
    RequestProcessor::process_buy_ammo_request(credentials, weapon_type, match);
}
