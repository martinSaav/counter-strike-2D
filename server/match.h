#ifndef GAME_LOOP_H
#define GAME_LOOP_H
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "game/map.h"
#include "game/player.h"

#include "game_identification.h"
#include "match_status_dto.h"
#include "player_command.h"
#include "player_credentials.h"

#define max_number_of_players 10
#define map_width 400
#define map_height 400

struct MatchFull: public std::runtime_error {
    MatchFull(): std::runtime_error("Match is full") {}
};

struct MatchAlreadyStarted: public std::runtime_error {
    MatchAlreadyStarted(): std::runtime_error("Match has already started") {}
};

class Match: public Thread {
    std::map<PlayerCredentials, std::shared_ptr<Player>> players;
    Queue<PlayerCommand> commands_queue;
    std::vector<std::shared_ptr<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>>
            senders_queues;
    std::mutex mtx;
    bool match_started;
    int player_count;
    const int max_player_count;
    Map map;
    std::atomic<bool> has_finished;

    void process_command(const PlayerCommand& command);

    MatchStatusDTO get_match_status();

    void broadcast_match_status();

    void process_move_player(const std::shared_ptr<Player>& player, int x_mov, int y_mov);


    void broadcast_match_start();

    void wait_for_match_to_start();

    void run_game_loop();

public:
    Match():
            commands_queue(Queue<PlayerCommand>()),
            match_started(false),
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
