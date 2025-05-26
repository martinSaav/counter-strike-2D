#include "match.h"

#include <algorithm>
#include <ranges>
#include <utility>

#include "game/collision_detector.h"
#define rate 30
#define miliseconds_per_iteration (1000 / rate)
#define starting_position_x 0
#define starting_position_y 0
#define tiles_per_movement 5

GameIdentification Match::join_match(const std::string& username) {
    std::lock_guard<std::mutex> lck(mtx);
    if (player_count == max_player_count) {
        throw MatchFull();
    }
    player_count++;
    Player player(username, starting_position_x, starting_position_y);
    PlayerCredentials credentials(player_count);
    players.insert(std::pair{credentials, player});
    auto sender_queue = std::make_shared<Queue<MatchStatusDTO>>();
    const GameIdentification game_identification(commands_queue, sender_queue, credentials);
    senders_queues.push_back(std::move(sender_queue));
    return game_identification;
}


void Match::process_move_player(Player& player, const int x_mov, const int y_mov) {
    auto [old_x, old_y] = player.get_location();
    const int new_x = old_x + x_mov;
    const int new_y = old_y + y_mov;
    if (!map.check_if_position_is_in_range(new_x, new_y)) {
        return;
    }
    for (const std::vector<Structure> structures = map.get_structures_near_player(player);
         const auto& structure: structures) {
        if (CollisionDetector::check_collision_between_player_and_structure(player, structure)) {
            return;
        }
    }
    try {
        const Position new_pos(new_x, new_y);
        player.set_location(new_pos);
    } catch (const InvalidPosition&) {}
}


void Match::process_command(const PlayerCommand command) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = command.credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    Player& player = player_p->second;
    switch (command.command_type) {
        case Action::MoveLeft: {
            return process_move_player(player, -tiles_per_movement, 0);
        }

        case Action::MoveRight: {
            return process_move_player(player, tiles_per_movement, 0);
        }
        case Action::MoveUp: {
            return process_move_player(player, 0, -tiles_per_movement);
        }
        case Action::MoveDown: {
            return process_move_player(player, 0, +tiles_per_movement);
        }
        default:
            break;
    }
}

MatchStatusDTO Match::get_match_status() {
    std::vector<PlayerDTO> player_dtos;
    auto players_view = players | std::views::values;
    std::ranges::transform(players_view, std::back_inserter(player_dtos),
                           [](const Player& player) { return player.get_player_info(); });
    return MatchStatusDTO{std::move(player_dtos)};
}


void Match::broadcast_match_status() {
    std::lock_guard<std::mutex> lck(mtx);
    const MatchStatusDTO match_status = get_match_status();
    auto it = senders_queues.begin();
    while (it != senders_queues.end()) {
        try {
            (*it)->push(match_status);
        } catch (const ClosedQueue&) {
            it = senders_queues.erase(it);
            continue;
        }
        ++it;
    }
}


void Match::run() {
    try {
        auto start = std::chrono::system_clock::now();
        while (should_keep_running()) {
            if (PlayerCommand command; commands_queue.try_pop(command)) {
                process_command(command);
            }
            broadcast_match_status();
            auto end = std::chrono::system_clock::now();
            const auto elapsed =
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            auto rest = miliseconds_per_iteration - elapsed;
            if (rest >=
                0) {  // Si esta adelantado duerme, si no pasa directamente a la proxima iteracion
                std::this_thread::sleep_for(std::chrono::milliseconds(rest));
            }
            start += std::chrono::milliseconds(
                    miliseconds_per_iteration);  // Sumandole el tiempo exacto que debe
            // tardar me aseguro que si duerme de mas, cuando se calcule end se entara que esta
            // atrasado y no dormira.
        }
    } catch (const ClosedQueue&) {}
    commands_queue.close();
}

int Match::get_player_count() const { return player_count; }


int Match::get_max_player_count() const { return max_player_count; }


void Match::stop() { Thread::stop(); }
