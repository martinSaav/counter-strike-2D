#ifndef MATCH_INFO_H
#define MATCH_INFO_H

#include <memory>
#include <utility>
#include <variant>

#include "commands/player_command.h"
#include "common/catedra/queue.h"

#include "game_ready_notification.h"
#include "match_status_dto.h"
#include "player_credentials.h"


class GameIdentification {
public:
    Queue<std::shared_ptr<PlayerCommand>>& command_queue;
    std::shared_ptr<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>> sender_queue;
    PlayerCredentials credentials;
    GameIdentification(Queue<std::shared_ptr<PlayerCommand>>& command_queue,
                       std::shared_ptr<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>
                               sender_queue,
                       const PlayerCredentials credentials):
            command_queue(command_queue),
            sender_queue(std::move(sender_queue)),
            credentials(credentials) {}
};


#endif  // MATCH_INFO_H
