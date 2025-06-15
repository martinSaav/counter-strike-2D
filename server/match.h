#ifndef GAME_LOOP_H
#define GAME_LOOP_H
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "commands/player_command.h"
#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "game/map.h"
#include "game/player.h"

#include "game_clock.h"
#include "game_identification.h"
#include "game_manager.h"
#include "match_status_dto.h"
#include "movement_type.h"
#include "player_credentials.h"
#include "request_processor.h"
#define max_number_of_players 10
#define map_width 155
#define map_height 285

struct MatchFull: public std::runtime_error {
    MatchFull(): std::runtime_error("Match is full") {}
};

struct MatchAlreadyStarted: public std::runtime_error {
    MatchAlreadyStarted(): std::runtime_error("Match has already started") {}
};

class Match: public Thread {
    friend class RequestProcessor;
    std::map<PlayerCredentials, std::shared_ptr<Player>> players;
    Queue<std::shared_ptr<PlayerCommand>> commands_queue;
    std::vector<std::shared_ptr<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>>
            senders_queues;
    std::mutex mtx;
    bool match_started;
    int player_count;
    const int max_player_count;
    Map map;
    GameClock game_clock;
    GameManager game_manager;
    std::atomic<bool> has_finished;

    MatchStatusDTO get_match_status();

    void broadcast_match_status();

    void process_move_player(const std::shared_ptr<Player>& player, int x_mov, int y_mov);

    void process_movement_request(PlayerCredentials credentials, MovementType command,
                                  Position aim_pos);

    void process_shoot(const std::shared_ptr<Player>& player, const Position& position);

    void process_shoot_request(PlayerCredentials credentials, Position aim_pos);

    void process_defuse(const std::shared_ptr<Player>& player);

    void process_defuse_request(PlayerCredentials credentials);

    void process_reload(const std::shared_ptr<Player>& player);

    void process_reload_request(PlayerCredentials credentials);

    void process_pick_weapon(const std::shared_ptr<Player>& player);

    void process_pick_weapon_request(PlayerCredentials credentials);

    void process_leave_match(const std::shared_ptr<Player>& player);

    void process_leave_match_request(PlayerCredentials credentials);

    void process_change_skin(const std::shared_ptr<Player>& player, PlayerSkin new_skin) const;

    void process_change_skin_request(PlayerCredentials credentials, PlayerSkin new_skin);

    void process_game_ready();

    void process_game_ready_request();

    void process_buy_weapon(const std::shared_ptr<Player>& player, Weapon weapon);

    void process_buy_weapon_request(PlayerCredentials credentials, Weapon weapon);

    void process_switch_weapon(const std::shared_ptr<Player>& player, GunType gun_type);

    void process_switch_weapon_request(PlayerCredentials credentials, GunType gun_type);

    void broadcast_match_start();

    void wait_for_match_to_start();

    void setup_round_start();

    void update_game();

    void run_game_loop();

    void wait_for_players_to_leave_match();

public:
    Match():
            commands_queue(Queue<std::shared_ptr<PlayerCommand>>()),
            match_started(false),
            player_count(0),
            max_player_count(max_number_of_players),
            map(map_width, map_height),
            game_manager(map, game_clock) {}
    GameIdentification join_match(const std::string& username);
    void run() override;
    void stop() override;
    [[nodiscard]] int get_player_count() const;
    [[nodiscard]] int get_max_player_count() const;
    [[nodiscard]] bool has_match_finished() const;
};

#endif  // GAME_LOOP_H
