//
// Created by matias on 15/05/25.
//

#ifndef SENDER_H
#define SENDER_H
#include <memory>
#include <utility>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "common/protocol.h"

#include "match_status_dto.h"

class Sender: public Thread {
    Protocol& protocol;
    std::shared_ptr<Queue<MatchStatusDTO>> sender_queue;

public:
    Sender(Protocol& protocol, std::shared_ptr<Queue<MatchStatusDTO>> sender_queue):
            protocol(protocol), sender_queue(std::move(sender_queue)) {}
    void run() override;
    void stop() override;
};

#endif  // SENDER_H
