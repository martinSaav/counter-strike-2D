//
// Created by matias on 16/05/25.
//

#ifndef MATCH_INFO_H
#define MATCH_INFO_H
#include <utility>

#include "common/catedra/queue.h"

#include "events.h"
#include "match_status_dto.h"
#include "player_command.h"
#include "player_credentials.h"


class GameIdentification {
public:
    Queue<PlayerCommand>& command_queue;
    Queue<MatchStatusDTO>& sender_queue;
    PlayerCredentials& credentials;
    GameIdentification(Queue<PlayerCommand>& event_queue, Queue<MatchStatusDTO>& sender_queue,
                       PlayerCredentials& credentials):
            command_queue(event_queue), sender_queue(sender_queue), credentials(credentials) {}
};


#endif  // MATCH_INFO_H
