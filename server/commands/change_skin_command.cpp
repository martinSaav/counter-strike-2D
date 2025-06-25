//
// Created by matias on 12/06/25.
//

#include "change_skin_command.h"

#include "match.h"

ChangeSkinCommand::ChangeSkinCommand(const PlayerCredentials credentials,
                                     const PlayerSkin new_skin):
        credentials(credentials), new_skin(new_skin) {}


void ChangeSkinCommand::process_command(Match* match) {
    RequestProcessor::process_change_skin_request(credentials, new_skin, match);
}
