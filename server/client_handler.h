//
// Created by matias on 15/05/25.
//

#ifndef SERVER_CLIENT_HANDLER_H_
#define SERVER_CLIENT_HANDLER_H_
#include <utility>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "common/protocol.h"
#include "server/events.h"
#include "server/lobby.h"

class ClientHandler: public Thread {
    Protocol protocol;
    Lobby& lobby;
    Queue<CommandTypes> receiver_queue;

    //   GameIdentification pick_match();

public:
    explicit ClientHandler(Protocol&& protocol, Lobby& lobby):
            protocol(std::move(protocol)), lobby(lobby) {}
    void run() override;
    void stop() override;
};

#endif  // SERVER_CLIENT_HANDLER_H_
