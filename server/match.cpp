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
    if (match_started) {
        throw MatchAlreadyStarted();
    }
    player_count++;
    Team player_team;
    if (player_count % 2 == 0) {
        player_team = Team::CounterTerrorists;
    } else {
        player_team = Team::Terrorists;
    }
    auto player = std::make_shared<Player>(username, starting_position_x + 35 * (player_count - 1),
                                           starting_position_y, player_team);
    PlayerCredentials credentials(player_count);
    players.insert(std::pair{credentials, player});
    auto sender_queue =
            std::make_shared<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>();
    GameIdentification game_identification(commands_queue, sender_queue, credentials);
    senders_queues.push_back(std::move(sender_queue));
    map.add_player(player);
    return game_identification;
}


void Match::process_move_player(const std::shared_ptr<Player>& player, const int x_mov,
                                const int y_mov) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    auto [old_x, old_y] = player->get_location();
    const int new_x = old_x + x_mov;
    const int new_y = old_y + y_mov;
    if (!map.check_if_position_is_in_range(new_x, new_y)) {
        return;
    }
    for (const std::vector<Structure> structures = map.get_structures_near_player(player);
         const auto& structure: structures) {
        if (CollisionDetector::check_collision_between_player_and_structure(new_x, new_y,
                                                                            structure)) {
            return;
        }
    }
    if (const auto& near_players = map.get_near_players(player);
        std::ranges::any_of(near_players, [&](const auto& p) {
            return CollisionDetector::check_collision_between_players(new_x, new_y, *p);
        })) {
        return;
    }
    try {
        const Position new_pos(new_x, new_y);
        auto new_chunks = Map::calculate_player_chunks(new_x, new_y);
        player->set_location(new_pos, std::move(new_chunks));
    } catch (const InvalidPosition&) {}
}


void Match::process_shoot_request(const std::shared_ptr<Player>& player, const Position& position) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    if (auto [x, y] = position.get_position(); map.check_if_position_is_in_range(x, y)) {
        player->shoot(position);
    }
}


void Match::process_defuse_request(const std::shared_ptr<Player>& player) const {
    game_manager.start_defusing(player);
}


void Match::process_reload_request(const std::shared_ptr<Player>& player) const {
    player->reload();
}


void Match::process_pick_weapon_request(const std::shared_ptr<Player>& player) {
    game_manager.pick_weapon(player);
}


void Match::process_leave_match_request(std::shared_ptr<Player> player) {
    map.remove_player(player);
    auto it = players.begin();
    while (it != players.end()) {
        if (it->second == nullptr) {
            it = players.erase(it);
            player_count--;
            continue;
        }
        if (it->second == player) {
            players.erase(it);
            player_count--;
            return;
        }
        ++it;
    }
}


void Match::process_command(const PlayerCommand& command) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = command.credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    if (command.position.has_value()) {
        auto [x, y] = command.position.value().get_position();
        player->set_aim_pos(x, y);
    }
    switch (command.command_type) {
        case CommandType::MoveLeft: {
            return process_move_player(player, -tiles_per_movement, 0);
        }

        case CommandType::MoveRight: {
            return process_move_player(player, tiles_per_movement, 0);
        }
        case CommandType::MoveUp: {
            return process_move_player(player, 0, -tiles_per_movement);
        }
        case CommandType::MoveDown: {
            return process_move_player(player, 0, +tiles_per_movement);
        }
        case CommandType::Shoot: {
            return process_shoot_request(player, command.position.value());
        }
        case CommandType::PlantBomb: {
            return process_shoot_request(player, command.position.value());
        }
        case CommandType::DefuseBomb: {
            return process_defuse_request(player);
        }
        case CommandType::Reload: {
            return process_reload_request(player);
        }
        case CommandType::EquipWeapon: {
            return process_pick_weapon_request(player);
        }
        case CommandType::LeaveMatch: {
            return process_leave_match_request(player);
        }
        default:
            break;
    }
}

MatchStatusDTO Match::get_match_status() {
    std::vector<PlayerDTO> player_dtos;
    auto players_view = players | std::views::values;
    std::ranges::transform(
            players_view, std::back_inserter(player_dtos),
            [](const std::shared_ptr<Player>& player) { return player->get_player_info(); });
    TimeInformation info = game_manager.get_time_information();
    return MatchStatusDTO{std::move(player_dtos),
                          match_started,
                          has_finished,
                          info.round,
                          info.round_time,
                          info.round_started,
                          info.round_ended,
                          info.bomb_planted,
                          info.bomb_x,
                          info.bomb_y,
                          info.bomb_timer,
                          info.round_winner,
                          game_manager.get_match_winner(),
                          map.get_dropped_weapons()};
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


void Match::broadcast_match_start() {
    auto it = senders_queues.begin();
    while (it != senders_queues.end()) {
        try {
            (*it)->push(GameReadyNotification());
        } catch (const ClosedQueue&) {
            it = senders_queues.erase(it);
            continue;
        }
        ++it;
    }
}


void Match::wait_for_match_to_start() {
    while (true) {
        switch (PlayerCommand command = commands_queue.pop(); command.command_type) {
            case CommandType::StartMatch: {
                match_started = true;
                broadcast_match_start();
                return;
            }
            case CommandType::ChangeSkin: {
                const PlayerCredentials credentials = command.credentials;
                const PlayerSkin new_skin = command.new_skin.value();
                const auto player = players.find(credentials);
                if (player == players.end()) {
                    break;
                }
                player->second->set_skin(new_skin);
                break;
            }
            default:
                break;
        }
    }
}


void Match::setup_round_start() {
    game_clock.reset();
    if (game_manager.has_to_switch_sides()) {
        for (const auto& player: players | std::views::values) {
            player->switch_team();
        }
    }
    if (game_manager.has_ended()) {
        has_finished = true;
    }
    int terorrist_spawn_x = player_hitbox_width;
    int counter_spawn_x = map_width - player_hitbox_width;
    std::vector<std::shared_ptr<Player>> players_vector;
    for (const auto& player: players | std::views::values) {
        player->restore();
        players_vector.push_back(player);
        if (player->get_team() == Team::Terrorists) {
            constexpr int terorrist_spawn_y = 0 + player_hitbox_height;
            const Position new_pos(terorrist_spawn_x, terorrist_spawn_y);
            player->set_location(new_pos,
                                 map.calculate_player_chunks(terorrist_spawn_x, terorrist_spawn_y));
            terorrist_spawn_x += player_hitbox_width;
        } else {
            constexpr int counter_spawn_y = map_height - player_hitbox_height;
            const Position new_pos(counter_spawn_x, counter_spawn_y);
            player->set_location(new_pos,
                                 map.calculate_player_chunks(counter_spawn_x, counter_spawn_y));
            counter_spawn_x -= player_hitbox_width;
        }
    }
    game_manager.give_bomb_to_random_player(players_vector);
}


void Match::update_game() {
    std::lock_guard<std::mutex> lck(mtx);
    try {
        game_clock.advance(miliseconds_per_iteration);
    } catch (const RoundAlreadyFinished&) {
        game_manager.advance_round();
        setup_round_start();
        return;
    }

    std::vector<std::shared_ptr<Player>> players_vector;
    for (const auto& player: players | std::views::values) {
        player->update(game_manager);
        players_vector.push_back(player);
    }

    game_manager.check_winning_cond(players_vector);
}


void Match::run_game_loop() {
    auto start = std::chrono::system_clock::now();
    while (should_keep_running() && !has_finished) {
        if (PlayerCommand command; commands_queue.try_pop(command)) {
            process_command(command);
        }
        update_game();
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
}


void Match::wait_for_players_to_leave_match() {
    while (player_count > 0) {
        try {
            switch (const PlayerCommand command = commands_queue.pop(); command.command_type) {
                case CommandType::LeaveMatch: {
                    player_count--;
                    return;
                }
                default:
                    break;
            }
        } catch (const ClosedQueue&) {
            return;
        }
    }
}


void Match::run() {
    wait_for_match_to_start();
    try {
        setup_round_start();
        run_game_loop();
    } catch (const ClosedQueue&) {
        return;
    }
    wait_for_players_to_leave_match();
}

int Match::get_player_count() const { return player_count; }


int Match::get_max_player_count() const { return max_player_count; }


void Match::stop() {
    Thread::stop();
    has_finished = true;
    commands_queue.close();
}


bool Match::has_match_finished() const { return has_finished && player_count == 0; }
