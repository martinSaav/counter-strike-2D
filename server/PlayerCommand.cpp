#include "PlayerCommand.h"

std::set<CommandTypes> PlayerCommand::get_player_commands_set() {
    std::set<CommandTypes> player_commands = {CommandTypes::moveUp, CommandTypes::moveDown,
                                              CommandTypes::moveLeft, CommandTypes::moveRight};
    return player_commands;
}

PlayerCommand::PlayerCommand(const PlayerCredentials& credentials, const CommandTypes type):
        credentials(credentials), type(type) {
    auto player_commands = get_player_commands_set();
    if (!player_commands.contains(type)) {}
}
