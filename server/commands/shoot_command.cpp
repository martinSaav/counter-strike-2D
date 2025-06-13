//
// Created by matias on 12/06/25.
//

#include "shoot_command.h"

#include "match.h"

ShootCommand::ShootCommand(const PlayerCredentials credentials, const Position aim_pos):
        credentials(credentials), aim_pos(aim_pos) {}

void ShootCommand::process_command(Match* match) {
    RequestProcessor::process_shoot_request(credentials, aim_pos, match);
}
