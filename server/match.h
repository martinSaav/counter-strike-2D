#ifndef GAME_LOOP_H
#define GAME_LOOP_H
#include <map>
#include <string>
#include <vector>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"

#include "events.h"
#include "game_identification.h"
#include "match_status_dto.h"
#include "player.h"
#include "player_credentials.h"

class Match: public Thread {
    std::map<PlayerCredentials, Player> players;
    Queue<CommandTypes> commands_queue;
    std::vector<Queue<MatchStatusDTO>> senders_queues;
    std::mutex mtx;
    int player_count;

    void process_command(CommandTypes command);

public:
    Match(): commands_queue(Queue<CommandTypes>()), player_count(0) {}
    GameIdentification join_match(const std::string& username);
    void run() override;
    void stop() override;
};

#endif  // GAME_LOOP_H
