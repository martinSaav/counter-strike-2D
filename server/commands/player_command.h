//
// Created by matias on 12/06/25.
//

#ifndef PLAYER_COMMAND_H
#define PLAYER_COMMAND_H
#include <stdexcept>

struct InvalidCommandType: public std::runtime_error {
    InvalidCommandType(): std::runtime_error("Invalid Command type") {}
};

class Match;

class PlayerCommand {
public:
    virtual ~PlayerCommand() = default;
    virtual void process_command(Match* match) = 0;
};


#endif  // PLAYER_COMMAND_H
