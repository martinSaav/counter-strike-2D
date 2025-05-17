//
// Created by matias on 15/05/25.
//

#ifndef SENDER_H
#define SENDER_H
#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "common/protocol.h"

#include "client_handler.h"
#include "match_status_dto.h"

class Sender: public Thread {
    Protocol& protocol;
    Queue<MatchStatusDTO>& sender_queue;
    ClientHandler& client_handler;

    void notify_error_to_client_handler();

public:
    Sender(Protocol& protocol, Queue<MatchStatusDTO>& sender_queue, ClientHandler& client_handler):
            protocol(protocol), sender_queue(sender_queue), client_handler(client_handler) {}
    void run() override;
    void stop() override;
};

#endif  // SENDER_H
