//
// Created by matias on 12/06/25.
//

#include "game_ready_command.h"

#include "match.h"

GameReadyCommand::GameReadyCommand(const PlayerCredentials credentials): credentials(credentials) {}


void GameReadyCommand::process_command(Match* match) {
    RequestProcessor::process_game_ready_request(match);
}
