//
// Created by matias on 13/06/25.
//

#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H
#include <memory>
#include <stdexcept>

#include "commands/player_command.h"
#include "common/message.h"

#include "player_credentials.h"

struct InvalidMessageType: public std::runtime_error {
    InvalidMessageType(): std::runtime_error("Invalid message type") {}
};

struct InvalidActionType: public std::runtime_error {
    InvalidActionType(): std::runtime_error("Invalid action type") {}
};

class MessageParser {
    PlayerCredentials credentials;
    std::shared_ptr<PlayerCommand> parse_game_ready();
    std::shared_ptr<PlayerCommand> parse_buy_weapon(const std::unique_ptr<Message>& message);
    std::shared_ptr<PlayerCommand> parse_select_skin(const std::unique_ptr<Message>& message);
    std::shared_ptr<PlayerCommand> parse_disconnect();
    std::shared_ptr<PlayerCommand> parse_action(const std::unique_ptr<Message>& message);

public:
    explicit MessageParser(const PlayerCredentials& credentials);
    std::shared_ptr<PlayerCommand> parse_message(const std::unique_ptr<Message>& message);
};


#endif  // MESSAGE_PARSER_H
