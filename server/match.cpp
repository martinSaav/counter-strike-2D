#include "match.h"

#include <algorithm>
#include <ranges>
#include <utility>
#define rate 30
#define miliseconds_per_iteration (1000 / rate)

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

void Match::process_command(const PlayerCommand command) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = command.credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    Player& player = player_p->second;
    switch (command.command_type) {
        case CommandTypes::moveLeft: {
            auto [x, y] = player.get_location();
            player.set_location(x - 1, y);
            break;
        }

        case CommandTypes::moveRight: {
            auto [x, y] = player.get_location();
            player.set_location(x + 1, y);
            break;
        }
        case CommandTypes::moveUp: {
            auto [x, y] = player.get_location();
            player.set_location(x, y - 1);
        }
        case CommandTypes::moveDown: {
            auto [x, y] = player.get_location();
            player.set_location(x, y + 1);
        }
        default:
    }
}

MatchStatusDTO Match::get_match_status() {
    std::vector<PlayerDTO> player_dtos;
    auto players_view = players | std::views::values;
    std::transform(players_view.begin(), players_view.end(), std::back_inserter(player_dtos),
                   [](const Player& player) { return player.get_player_info(); });
    return MatchStatusDTO{std::move(player_dtos)};
}


void Match::broadcast_match_status() {
    std::lock_guard<std::mutex> lck(mtx);
    const MatchStatusDTO match_status = get_match_status();
    auto it = senders_queues.begin();
    while (it != senders_queues.end()) {
        try {
            it->push(match_status);
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


void Match::stop() { Thread::stop(); }
