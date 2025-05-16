#include "match.h"

#include <utility>
GameIdentification Match::join_match(const std::string& username) {
    std::lock_guard<std::mutex> lck(mtx);
    player_count++;
    Player player(username, 0, 0);
    PlayerCredentials credentials(player_count);
    players.insert(std::pair{credentials, player});
    auto sender_queue = Queue<MatchStatusDTO>();
    const GameIdentification game_identification(commands_queue, sender_queue, credentials);
    senders_queues.push_back(std::move(sender_queue));
    return game_identification;
}

void Match::process_command(CommandTypes command) {

    //  switch (command) {
    //      case
    //  }
}


void Match::run() {
    try {
        while (should_keep_running()) {
            CommandTypes command;
            if (commands_queue.try_pop(command)) {
                process_command(command);
            }
        }
    } catch (const ClosedQueue&) {}
}


void Match::stop() { Thread::stop(); }
