//
// Created by matias on 13/06/25.
//

#include "defuse_bomb_command.h"

#include "match.h"

DefuseBombCommand::DefuseBombCommand(const PlayerCredentials credentials):
        credentials(credentials) {}


void DefuseBombCommand::process_command(Match* match) {
    match->process_defuse_request(credentials);
}
