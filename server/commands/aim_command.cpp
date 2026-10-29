#include "aim_command.h"

#include "match.h"

AimCommand::AimCommand(const PlayerCredentials credentials, const Position aim_pos):
        credentials(credentials), aim_pos(aim_pos) {}


void AimCommand::process_command(Match* match) { match->process_aim_request(credentials, aim_pos); }
