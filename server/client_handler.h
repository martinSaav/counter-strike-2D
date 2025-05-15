//
// Created by matias on 15/05/25.
//

#ifndef SERVER_CLIENT_HANDLER_H_
#define SERVER_CLIENT_HANDLER_H_
#include <utility>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "server/events.h"
#include "server/lobby.h"
#include "server/match_status_dto.h"
class ClientHandler: public Thread {
    CommunicationProtocol protocol;
    Lobby& lobby;
    Queue<CommandTypes> receiver_queue;
    Queue<MatchStatusDTO> sender_queue;

public:
    explicit ClientHandler(CommunicationProtocol&& protocol, Lobby& lobby):
            protocol(std::move(protocol)), lobby(lobby) {}
    void run() override;
    void stop() override;
};

#endif  // SERVER_CLIENT_HANDLER_H_
