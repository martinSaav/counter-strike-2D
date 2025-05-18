#include "player_command.h"

PlayerCommand::PlayerCommand():
        credentials(invalid_credential), command_type(CommandTypes::invalid) {}


std::set<CommandTypes> PlayerCommand::get_player_commands_set() {
    std::set<CommandTypes> player_commands = {CommandTypes::moveUp, CommandTypes::moveDown,
                                              CommandTypes::moveLeft, CommandTypes::moveRight};
    return player_commands;
}

PlayerCommand::PlayerCommand(const PlayerCredentials& credentials, const CommandTypes type):
        credentials(credentials), command_type(type) {
    if (const auto player_commands = get_player_commands_set(); !player_commands.contains(type)) {
        throw InvalidCommandType();
    }
}
