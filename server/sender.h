//
// Created by matias on 15/05/25.
//

#ifndef SENDER_H
#define SENDER_H
#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "common/protocol.h"

#include "match_status_dto.h"

class Sender: public Thread {
    Protocol protocol;
    Queue<MatchStatusDTO> sender_queue;

    void run() override;
    void stop() override;
};

#endif  // SENDER_H
