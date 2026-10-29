#ifndef GAME_LOOP_H
#define GAME_LOOP_H
#include <functional>
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

struct MatchFull: public std::runtime_error {
    MatchFull(): std::runtime_error("Match is full") {}
};

struct MatchAlreadyStarted: public std::runtime_error {
    MatchAlreadyStarted(): std::runtime_error("Match has already started") {}
};

class Match: public Thread {
    // Cada partida tiene su propia configuracion (con el mapa elegido); el Sender de cada
    // jugador comparte el puntero para mandarsela al cliente.
    std::shared_ptr<GameConfig> game_config_ptr;
    GameConfig& game_config;
    const int tiles_per_movement;
    const int rate;
    const double miliseconds_per_iteration;
    std::map<PlayerCredentials, std::shared_ptr<Player>> players;
    Queue<std::shared_ptr<PlayerCommand>> commands_queue;
    std::vector<std::shared_ptr<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>>
            senders_queues;
    std::map<PlayerCredentials,
             std::shared_ptr<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>>
            senders_by_player;
    std::mutex mtx;
    std::atomic<bool> match_started;
    int player_count;
    const int max_player_count;
    Map map;
    GameClock game_clock;
    GameManager game_manager;
    std::atomic<bool> has_finished;

    MatchStatusDTO get_match_status();

    void broadcast_match_status();

    void try_half_movement(const std::shared_ptr<Player>& player, int x_mov, int y_mov);

    void process_move_player(const std::shared_ptr<Player>& player, int x_mov, int y_mov);

    void process_movement(const std::shared_ptr<Player>& player, MovementType command);

    // Busca al jugador (con el mutex tomado) y ejecuta la accion solo si sigue en la partida
    void with_player(PlayerCredentials credentials,
                     const std::function<void(const std::shared_ptr<Player>&)>& action);

    void process_shoot(const std::shared_ptr<Player>& player, const Position& position);

    void process_defuse(const std::shared_ptr<Player>& player);

    void process_reload(const std::shared_ptr<Player>& player);

    void process_pick_weapon(const std::shared_ptr<Player>& player);

    void process_leave_match(const std::shared_ptr<Player>& player);

    void process_change_skin(const std::shared_ptr<Player>& player, PlayerSkin new_skin) const;

    void process_game_ready(PlayerCredentials credentials);

    void process_buy_weapon(const std::shared_ptr<Player>& player, Weapon weapon);

    void process_switch_weapon(const std::shared_ptr<Player>& player, GunType gun_type);

    void process_buy_ammo(const std::shared_ptr<Player>& player, GunType gun_type) const;

    void broadcast_match_start();

    void wait_for_match_to_start();

    void setup_round_start();

    void update_game();

    void reset_players_actions();

    void run_game_loop();

    void wait_for_players_to_leave_match();

public:
    // Pedidos de los jugadores: los encolan los comandos (server/commands) y se procesan en el
    // hilo de la partida
    void process_movement_request(PlayerCredentials credentials, MovementType command,
                                  Position aim_pos);

    void process_shoot_request(PlayerCredentials credentials, Position aim_pos);

    void process_defuse_request(PlayerCredentials credentials);

    void process_reload_request(PlayerCredentials credentials);

    void process_pick_weapon_request(PlayerCredentials credentials);

    void process_drop_weapon_request(PlayerCredentials credentials);

    void process_aim_request(PlayerCredentials credentials, Position aim_pos);

    void process_leave_match_request(PlayerCredentials credentials);

    void process_change_skin_request(PlayerCredentials credentials, PlayerSkin new_skin);

    void process_game_ready_request(PlayerCredentials credentials);

    void process_buy_weapon_request(PlayerCredentials credentials, Weapon weapon);

    void process_switch_weapon_request(PlayerCredentials credentials, GunType gun_type);

    void process_buy_ammo_request(PlayerCredentials credentials, GunType gun_type);

    Match(const GameConfig& base_config, const std::string& map_name):
            game_config_ptr(
                    std::make_shared<GameConfig>(base_config, base_config.find_map(map_name))),
            game_config(*game_config_ptr),
            tiles_per_movement(game_config.tiles_per_movement),
            rate(game_config.game_rate),
            miliseconds_per_iteration(static_cast<double>(1000) / rate),
            commands_queue(Queue<std::shared_ptr<PlayerCommand>>()),
            match_started(false),
            player_count(0),
            max_player_count(game_config.ct_amount + game_config.tt_amount),
            map(game_config),
            game_clock(game_config),
            game_manager(map, game_clock, game_config) {}
    GameIdentification join_match(const std::string& username);
    void run() override;
    void stop() override;
    [[nodiscard]] int get_player_count() const;
    [[nodiscard]] int get_max_player_count() const;
    [[nodiscard]] bool has_match_finished() const;

    [[nodiscard]] bool has_match_started() const;

    [[nodiscard]] const std::string& get_map_name() const {
        return game_config.map_config.map_name;
    }
};

#endif  // GAME_LOOP_H
