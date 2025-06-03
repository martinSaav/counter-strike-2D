#include "player_command.h"

PlayerCommand::PlayerCommand():
        credentials(invalid_credential), command_type(CommandType::MoveDown) {}


PlayerCommand::PlayerCommand(const PlayerCredentials& credentials, const CommandType type):
        credentials(credentials), command_type(type) {}
