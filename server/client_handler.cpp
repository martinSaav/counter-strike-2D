//
// Created by matias on 15/05/25.
//

#include "client_handler.h"

#include <list>
#include <vector>

#include "common/dto/login_request.h"
#include "common/game_info.h"
#include "common/message.h"
#define generic_username "username"
#include "catedra/liberror.h"
#include "common/dto/create_game_request.h"
#include "common/dto/create_game_response.h"
#include "common/dto/game_list_response.h"
#include "common/dto/join_game_request.h"
#include "common/dto/join_game_response.h"
#include "common/dto/map_names_response.h"
#include "common/dto/player_action.h"
#include "common/dto/select_skin_request.h"

#include "skin_translator.h"

std::string ClientHandler::handle_login() {
    while (true) {
        const auto message = protocol.recv_message();
        switch (message->type()) {
            case MessageType::LoginRequest: {
                const auto login_message = dynamic_cast<LoginRequest*>(message.get());
                return login_message->get_username();
            }
            default:  // Mientras no reciba el mensaje de login continuo recibiendo mensajes hasta
                      // recibirlo.
                break;
        }
    }
}

void ClientHandler::handle_list_matches_request() {
    const std::vector<MatchDTO> matches = lobby.list_matches();
    std::list<GameInfo> games;
    for (const auto& match: matches) {
        GameInfo game;
        game.current_players = match.number_of_players;
        game.map_name = match.map_name;
        game.name = match.match_name;
        game.max_players = match.max_players;
        games.push_back(game);
    }
    const GameListResponse message(games);
    protocol.send_message(message);
}

void ClientHandler::handle_map_names_request() {
    while (true) {
        const auto message = protocol.recv_message();
        switch (message->type()) {
            case MessageType::MapNamesRequest: {
                std::list<std::string> map_names;
                map_names.emplace_back(generic_map_name);

                const MapNamesResponse response(std::move(map_names));
                protocol.send_message(response);
                return;
            }
            default:  // Mientras no reciba el mensaje de pedido de mapas continuo recibiendo
                      // mensajes hasta
                // recibirlo.
                break;
        }
    }
}


GameIdentification ClientHandler::handle_create_game_request(std::unique_ptr<Message>&& message) {
    const auto create_message = dynamic_cast<CreateGameRequest*>(message.get());
    const std::string game_name = create_message->get_game_name();
    const GameIdentification id = lobby.create_match(game_name, username);
    match_name = game_name;
    const CreateGameResponse response(true);
    protocol.send_message(response);
    return id;
}

GameIdentification ClientHandler::handle_join_game_request(std::unique_ptr<Message>&& message) {
    const auto join_message = dynamic_cast<JoinGameRequest*>(message.get());
    const std::string game_name = join_message->get_game_name();
    const GameIdentification id = lobby.join_match(game_name, username);
    match_name = game_name;
    const JoinGameResponse response(true);
    protocol.send_message(response);
    return id;
}


void ClientHandler::handle_disconnect_request(Queue<PlayerCommand>& command_queue,
                                              const PlayerCredentials& credentials) {
    command_queue.push(PlayerCommand(credentials, CommandType::LeaveMatch));
}


std::optional<GameIdentification> ClientHandler::pick_match() {
    while (should_keep_running()) {
        try {
            auto message = protocol.recv_message();
            switch (message->type()) {
                case MessageType::GameListRequest: {
                    handle_list_matches_request();
                    break;
                }
                case MessageType::CreateGameRequest: {
                    try {
                        return handle_create_game_request(std::move(message));
                    } catch (const MatchAlreadyExists&) {
                        protocol.send_message(CreateGameResponse(false));
                    }
                }
                case MessageType::JoinGameRequest: {
                    try {
                        return handle_join_game_request(std::move(message));
                    } catch (const MatchFull&) {
                        protocol.send_message(JoinGameResponse(false));
                    } catch (const MatchNotFound&) {
                        protocol.send_message(JoinGameResponse(false));
                    }
                }
                case MessageType::DisconnectRequest: {
                    return std::nullopt;
                }
                default:
                    break;
            }
        } catch (const std::exception& e) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}


CommandType ClientHandler::cast_action_to_command(const Action action) {
    switch (action) {
        case Action::MoveDown:
            return CommandType::MoveDown;
        case Action::MoveUp:
            return CommandType::MoveUp;
        case Action::MoveLeft:
            return CommandType::MoveLeft;
        case Action::MoveRight:
            return CommandType::MoveRight;
        case Action::Shoot:
            return CommandType::Shoot;
        case Action::DefuseBomb:
            return CommandType::DefuseBomb;
        case Action::EquipWeapon:
            return CommandType::EquipWeapon;
        case Action::PlantBomb:
            return CommandType::PlantBomb;
        case Action::Reload:
            return CommandType::Reload;
        case Action::Idle:
            return CommandType::Unknown;
        //case Action::SwitchWeapon:
        //    return CommandType::SwitchWeapon;
    }
    return CommandType::Unknown;
}


void ClientHandler::handle_player_action(Queue<PlayerCommand>& command_queue,
                                         const PlayerCredentials& credentials,
                                         const std::unique_ptr<Message>& message) {
    const auto player_action_message = dynamic_cast<PlayerAction*>(message.get());
    try {
        const Position position(player_action_message->get_pos_mouse_x(),
                                player_action_message->get_pos_mouse_y());
        const PlayerCommand player_command(
                credentials, cast_action_to_command(player_action_message->get_action()), position);
        command_queue.push(player_command);
    } catch (const InvalidPosition&) {}
}


void ClientHandler::handle_game_ready(Queue<PlayerCommand>& command_queue,
                                      const PlayerCredentials& credentials) {
    const PlayerCommand command(credentials, CommandType::StartMatch);
    command_queue.push(command);
}


void ClientHandler::handle_change_skin(Queue<PlayerCommand>& command_queue,
                                       const PlayerCredentials& credentials,
                                       const std::unique_ptr<Message>& message) {
    const auto change_skin_message = dynamic_cast<SelectSkinRequest*>(message.get());
    const PlayerSkin skin = SkinTranslator::string_to_code(change_skin_message->get_skin());
    command_queue.push(PlayerCommand(credentials, CommandType::ChangeSkin, skin));
}

void ClientHandler::handle_game(Queue<PlayerCommand>& command_queue,
                                const PlayerCredentials& credentials) {
    while (should_keep_running() && sender.value()->is_running()) {
        try {
            switch (auto message = protocol.recv_message(); message->type()) {
                case MessageType::PlayerAction: {
                    handle_player_action(command_queue, credentials, message);
                    break;
                }
                case MessageType::GameReadyRequest: {
                    handle_game_ready(command_queue, credentials);
                    break;
                }
                case MessageType::SelectSkinRequest: {
                    handle_change_skin(command_queue, credentials, message);
                    break;
                }
                case MessageType::DisconnectRequest: {
                    return handle_disconnect_request(command_queue, credentials);
                }
                default:
                    break;
            }
        } catch (const std::exception&) {
            return handle_disconnect_request(command_queue, credentials);
        }
    }
}


void ClientHandler::run() {
    try {
        username = handle_login();
        handle_map_names_request();
    } catch (const std::exception& e) {
        if (!should_keep_running()) {
            return;
        }
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        return;
    }
    const auto match_id_o = pick_match();
    if (!match_id_o.has_value()) {
        return;
    }
    GameIdentification match_id = match_id_o.value();
    sender = std::make_unique<Sender>(protocol, match_id.sender_queue);
    sender.value()->start();
    try {
        handle_game(match_id.command_queue, match_id.credentials);
    } catch (const ClosedQueue&) {
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unexpected exception: <unknown>\n";
    }

    if (sender.has_value()) {
        const auto& sender_v = sender.value();
        if (sender_v->is_alive()) {
            sender_v->stop();
        }
        sender_v->join();
    }
}

void ClientHandler::stop() {
    Thread::stop();
    protocol.kill();
}
