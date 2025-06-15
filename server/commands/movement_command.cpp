//
// Created by matias on 12/06/25.
//

#include "movement_command.h"

#include "match.h"
#include "request_processor.h"


MovementCommand::MovementCommand(const PlayerCredentials credentials, const MovementType command,
                                 const Position aim_pos):
        credentials(credentials), command(command), aim_pos(aim_pos) {}

void MovementCommand::process_command(Match* match) {
    RequestProcessor::process_movement_request(credentials, command, aim_pos, match);
}
