#include "match.h"

#include <algorithm>
#include <ranges>
#include <utility>

#include "game/collision_detector.h"

#include "gun_shop.h"

GameIdentification Match::join_match(const std::string& username) {
    std::lock_guard<std::mutex> lck(mtx);
    if (player_count == max_player_count) {
        throw MatchFull();
    }
    if (match_started) {
        throw MatchAlreadyStarted();
    }
    player_count++;
    Team player_team = game_manager.get_next_player_team();
    auto player = std::make_shared<Player>(username, game_config, 0, 0, player_team);
    PlayerCredentials credentials(player_count);
    players.insert(std::pair{credentials, player});
    auto sender_queue =
            std::make_shared<Queue<std::variant<MatchStatusDTO, GameReadyNotification>>>();
    GameIdentification game_identification(commands_queue, sender_queue, credentials);
    senders_queues.push_back(std::move(sender_queue));
    map.add_player(player);
    game_manager.add_player_to_team(player);
    return game_identification;
}


void Match::try_half_movement(const std::shared_ptr<Player>& player, int x_mov, int y_mov) {
    auto [old_x, old_y] = player->get_location();
    const int new_x = old_x + x_mov / 2;
    const int new_y = old_y + y_mov / 2;
    try {
        auto new_chunks = map.calculate_player_chunks(new_x, new_y);
        for (const std::vector<Structure> structures = map.get_structures_near_player(new_x, new_y);
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
        const Position new_pos(new_x, new_y);
        player->set_location(new_pos, std::move(new_chunks));
    } catch (const InvalidPosition&) {
    } catch (const PositionOutOfRange&) {}
}


void Match::process_move_player(const std::shared_ptr<Player>& player, const int x_mov,
                                const int y_mov) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    auto [old_x, old_y] = player->get_location();
    const int new_x = old_x + x_mov;
    const int new_y = old_y + y_mov;
    try {
        auto new_chunks = map.calculate_player_chunks(new_x, new_y);
        for (const std::vector<Structure> structures = map.get_structures_near_player(new_x, new_y);
             const auto& structure: structures) {
            if (CollisionDetector::check_collision_between_player_and_structure(new_x, new_y,
                                                                                structure)) {
                return try_half_movement(player, x_mov, y_mov);
            }
        }
        if (const auto& near_players = map.get_near_players(player);
            std::ranges::any_of(near_players, [&](const auto& p) {
                return CollisionDetector::check_collision_between_players(new_x, new_y, *p);
            })) {
            return try_half_movement(player, x_mov, y_mov);
        }
        const Position new_pos(new_x, new_y);
        player->set_location(new_pos, std::move(new_chunks));
    } catch (const InvalidPosition&) {
    } catch (const PositionOutOfRange&) {}
}


void Match::process_movement_request(PlayerCredentials credentials, MovementType command,
                                     Position aim_pos) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    auto [x, y] = aim_pos.get_position();
    player->set_aim_pos(x, y);
    switch (command) {
        case MovementType::MoveLeft: {
            return process_move_player(player, -tiles_per_movement, 0);
        }

        case MovementType::MoveRight: {
            return process_move_player(player, tiles_per_movement, 0);
        }
        case MovementType::MoveUp: {
            return process_move_player(player, 0, -tiles_per_movement);
        }
        case MovementType::MoveDown: {
            return process_move_player(player, 0, +tiles_per_movement);
        }
        default: {
            break;
        }
    }
}


void Match::process_shoot(const std::shared_ptr<Player>& player, const Position& position) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    if (auto [x, y] = position.get_position(); map.check_if_position_is_in_range(x, y)) {
        player->shoot(position, static_cast<float>(game_clock.get_time()));
    }
}

void Match::process_shoot_request(PlayerCredentials credentials, Position aim_pos) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_shoot(player, aim_pos);
}


void Match::process_defuse(const std::shared_ptr<Player>& player) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    game_manager.start_defusing(player);
}


void Match::process_defuse_request(const PlayerCredentials credentials) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_defuse(player);
}


void Match::process_reload(const std::shared_ptr<Player>& player) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    player->reload();
}


void Match::process_reload_request(PlayerCredentials credentials) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_reload(player);
}


void Match::process_pick_weapon(const std::shared_ptr<Player>& player) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    game_manager.pick_weapon(player);
}


void Match::process_pick_weapon_request(const PlayerCredentials credentials) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_pick_weapon(player);
}


void Match::process_leave_match(const std::shared_ptr<Player>& player) {
    map.remove_player(player);
    game_manager.remove_player_from_team(player);
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
            break;
        }
        ++it;
    }
    if (player_count <= 1) {
        has_finished = true;
    }
}


void Match::process_leave_match_request(const PlayerCredentials credentials) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_leave_match(player);
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


void Match::process_change_skin(const std::shared_ptr<Player>& player,
                                const PlayerSkin new_skin) const {
    if (!match_started) {
        player->set_skin(new_skin);
    }
}


void Match::process_change_skin_request(const PlayerCredentials credentials,
                                        const PlayerSkin new_skin) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_change_skin(player, new_skin);
}


void Match::process_game_ready() {
    if (!match_started) {
        match_started = true;
        broadcast_match_start();
    }
}


void Match::process_game_ready_request() { process_game_ready(); }


void Match::wait_for_match_to_start() {
    while (!match_started && !has_finished) {
        std::shared_ptr<PlayerCommand> command = commands_queue.pop();
        command->process_command(this);
    }
}

void Match::process_buy_weapon(const std::shared_ptr<Player>& player, const Weapon weapon) {
    if (!game_manager.can_player_buy()) {
        return;
    }
    try {
        std::unique_ptr<Gun> gun = GunShop::get_gun(weapon, game_config);
        player->buy_weapon(std::move(gun));
    } catch (const CantBuyWeapon&) {}
}


void Match::process_buy_weapon_request(const PlayerCredentials credentials, const Weapon weapon) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_buy_weapon(player, weapon);
}


void Match::process_switch_weapon(const std::shared_ptr<Player>& player, GunType gun_type) {
    if (!game_manager.can_player_move_or_shoot(player)) {
        return;
    }
    player->switch_weapon(gun_type);
}


void Match::process_switch_weapon_request(PlayerCredentials credentials, GunType gun_type) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_switch_weapon(player, gun_type);
}


void Match::process_buy_ammo(const std::shared_ptr<Player>& player, GunType gun_type) const {
    if (!game_manager.can_player_buy()) {
        return;
    }
    player->buy_ammo(gun_type);
}


void Match::process_buy_ammo_request(const PlayerCredentials credentials, const GunType gun_type) {
    std::lock_guard<std::mutex> lck(mtx);
    const PlayerCredentials player_credentials = credentials;
    const auto player_p = players.find(player_credentials);
    if (player_p == players.end()) {
        return;  // El player ya no se encuentra en la partida por lo tanto descarto la accion.
    }
    const std::shared_ptr<Player>& player = player_p->second;
    process_buy_ammo(player, gun_type);
}


void Match::setup_round_start() {
    game_clock.reset();
    if (game_manager.has_to_switch_sides()) {
        for (const auto& player: players | std::views::values) {
            player->switch_team();
        }
        game_manager.switch_sides();
    }
    if (game_manager.has_ended() || player_count == 0) {
        has_finished = true;
    }
    std::vector<std::shared_ptr<Player>> players_vector;
    for (const auto& player: players | std::views::values) {
        player->restore();
        players_vector.push_back(player);
    }
    game_manager.set_players_spawn(players_vector);
    map.clear_bomb();
    game_manager.give_bomb_to_random_player(players_vector);
}


void Match::update_game() {
    std::lock_guard<std::mutex> lck(mtx);
    try {
        game_clock.advance(miliseconds_per_iteration);
    } catch (const RoundAlreadyFinished&) {
        const std::vector players_vector((players | std::views::values).begin(),
                                         (players | std::views::values).end());
        game_manager.advance_round(players_vector);
        setup_round_start();
        return;
    }

    std::vector<std::shared_ptr<Player>> players_vector;
    for (const auto& player: players | std::views::values) {
        player->update(game_manager);
        players_vector.push_back(player);
    }
    game_manager.check_winning_cond(players_vector);
    if (game_clock.has_bomb_exploded() && !game_manager.exploded_bomb()) {
        game_manager.explode_bomb(players_vector);
    }
}


void Match::reset_players_actions() {
    for (const auto& player: players | std::views::values) {
        player->set_idle();
    }
}


void Match::run_game_loop() {
    using clock = std::chrono::steady_clock;
    using duration_ms = std::chrono::duration<double, std::milli>;

    const duration_ms frame_duration(miliseconds_per_iteration);

    while (should_keep_running() && !has_finished) {
        auto start = clock::now();

        std::shared_ptr<PlayerCommand> command;
        while (commands_queue.try_pop(command)) {
            command->process_command(this);
        }

        update_game();
        broadcast_match_status();
        reset_players_actions();

        auto end = clock::now();
        duration_ms elapsed = end - start;

        if (elapsed < frame_duration) {
            std::this_thread::sleep_for(frame_duration -
                                        elapsed);  // Si está adelantado duerme, si no pasa
                                                   // directamente a la proxima iteracion
        }
    }
}

void Match::wait_for_players_to_leave_match() {
    while (player_count > 0) {
        try {
            const std::shared_ptr<PlayerCommand> command = commands_queue.pop();
            command->process_command(this);
        } catch (const ClosedQueue&) {
            return;
        }
    }
}


void Match::run() {
    wait_for_match_to_start();
    if (has_finished) {
        return;
    }
    try {
        setup_round_start();
        run_game_loop();
    } catch (const ClosedQueue&) {
        return;
    } catch (...) {
        has_finished = true;
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
