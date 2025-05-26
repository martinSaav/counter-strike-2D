#ifndef GAME_LOOP_H
#define GAME_LOOP_H
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"

#include "game_identification.h"
#include "map.h"
#include "match_status_dto.h"
#include "player.h"
#include "player_command.h"
#include "player_credentials.h"

#define max_number_of_players 10
#define map_width 100
#define map_height 100

struct MatchFull: public std::runtime_error {
    MatchFull(): std::runtime_error("Match is full") {}
};

class Match: public Thread {
    std::map<PlayerCredentials, Player> players;
    Queue<PlayerCommand> commands_queue;
    std::vector<std::shared_ptr<Queue<MatchStatusDTO>>> senders_queues;
    std::mutex mtx;
    int player_count;
    const int max_player_count;
    Map map;
    std::atomic<bool> has_finished;

    void process_command(PlayerCommand command);

    MatchStatusDTO get_match_status();

    void broadcast_match_status();

    void process_move_player(Player& player, int x_mov, int y_mov);

public:
    Match():
            commands_queue(Queue<PlayerCommand>()),
            player_count(0),
            max_player_count(max_number_of_players),
            map(map_width, map_height) {}
    GameIdentification join_match(const std::string& username);
    void run() override;
    void stop() override;
    [[nodiscard]] int get_player_count() const;
    [[nodiscard]] int get_max_player_count() const;
};

#endif  // GAME_LOOP_H
