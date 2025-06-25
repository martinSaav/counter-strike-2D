//
// Created by matias on 12/06/25.
//

#include "leave_match_command.h"

#include "match.h"

LeaveMatchCommand::LeaveMatchCommand(const PlayerCredentials credentials):
        credentials(credentials) {}


void LeaveMatchCommand::process_command(Match* match) {
    RequestProcessor::process_leave_match_request(credentials, match);
}
