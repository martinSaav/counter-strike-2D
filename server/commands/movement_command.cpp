//
// Created by matias on 12/06/25.
//

#include "movement_command.h"

#include "match.h"
#include "request_processor.h"

std::set<CommandType> MovementCommand::get_valid_commands() {
    std::set<CommandType> valid_commands = {CommandType::MoveDown, CommandType::MoveUp,
                                            CommandType::MoveLeft, CommandType::MoveRight};
    return valid_commands;
}


MovementCommand::MovementCommand(const PlayerCredentials credentials, const CommandType command,
                                 const Position aim_pos):
        credentials(credentials), command(command), aim_pos(aim_pos) {
    if (const std::set<CommandType> valid_commands = get_valid_commands();
        !valid_commands.contains(command)) {
        throw InvalidCommandType();
    }
}

void MovementCommand::process_command(Match* match) {
    RequestProcessor::process_movement_request(credentials, command, aim_pos, match);
}
