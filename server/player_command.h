//
// Created by matias on 16/05/25.
//

#ifndef PLAYERCOMMAND_H
#define PLAYERCOMMAND_H

#include <optional>
#include <stdexcept>

#include "common/action.h"

#include "command_type.h"
#include "player_credentials.h"
#include "player_skin.h"
#include "position.h"

struct InvalidCommandType: public std::runtime_error {
    InvalidCommandType(): std::runtime_error("Invalid Command Type") {}
};

class PlayerCommand {
    friend class Match;
    PlayerCommand();

public:
    PlayerCredentials credentials;
    CommandType command_type;
    std::optional<Position> position;
    std::optional<PlayerSkin> new_skin;

    PlayerCommand(const PlayerCredentials& credentials, CommandType type);
    PlayerCommand(const PlayerCredentials& credentials, const CommandType type,
                  const Position& position):
            credentials(credentials), command_type(type), position(position) {}
    PlayerCommand(const PlayerCredentials& credentials, const CommandType type, PlayerSkin skin):
            credentials(credentials), command_type(type), new_skin(skin) {}
};


#endif  // PLAYERCOMMAND_H
