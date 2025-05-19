#include "player_command.h"

PlayerCommand::PlayerCommand(): credentials(invalid_credential), command_type(Action::MoveDown) {}


PlayerCommand::PlayerCommand(const PlayerCredentials& credentials, const Action type):
        credentials(credentials), command_type(type) {}
