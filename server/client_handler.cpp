//
// Created by matias on 15/05/25.
//

#include "client_handler.h"

#include <list>
#include <vector>

#include "common/dto/login_request.h"
#include "common/game.h"
#include "common/message.h"
#define generic_username "username"
#include "common/dto/create_game_request.h"
#include "common/dto/create_game_response.h"
#include "common/dto/game_list_response.h"
#include "common/dto/join_game_request.h"
#include "common/dto/join_game_response.h"
#include "common/dto/map_names_response.h"
#include "common/dto/player_action.h"

std::string ClientHandler::handle_login() {
    while (true) {
        const auto message = protocol.recv_message();
        switch (message->type()) {
            case MessageType::LoginRequest: {
                const auto login_message = dynamic_cast<LoginRequest*>(message.get());
                std::string username = login_message->get_username();
                return username;
            }
            default:  // Mientras no reciba el mensaje de login continuo recibiendo mensajes hasta
                      // recibirlo.
                break;
        }
    }
}

void ClientHandler::handle_list_matches_request() {
    const std::vector<MatchDTO> matches = lobby.list_matches();
    std::list<Game> games;
    for (const auto& match: matches) {
        Game game;
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
    const GameIdentification id = lobby.create_match(create_message->get_game_name(), username);
    const CreateGameResponse response(true);
    protocol.send_message(response);
    return id;
}

GameIdentification ClientHandler::handle_join_game_request(std::unique_ptr<Message>&& message) {
    const auto join_message = dynamic_cast<JoinGameRequest*>(message.get());
    const GameIdentification id = lobby.join_match(join_message->get_game_name(), username);
    const JoinGameResponse response(true);
    protocol.send_message(response);
    return id;
}


GameIdentification ClientHandler::pick_match() {
    while (true) {
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
            default:
                break;
        }
    }
}

void ClientHandler::handle_player_action(Queue<PlayerCommand>& command_queue,
                                         const PlayerCredentials& credentials,
                                         const std::unique_ptr<Message>& message) {
    const auto player_action_message = dynamic_cast<PlayerAction*>(message.get());
    const PlayerCommand player_command(credentials,
                                       static_cast<Action>(player_action_message->get_action()));
    command_queue.push(player_command);
}


void ClientHandler::handle_game(Queue<PlayerCommand>& command_queue,
                                const PlayerCredentials& credentials) {
    while (true) {
        auto message = protocol.recv_message();
        switch (message->type()) {
            case MessageType::PlayerAction: {
                handle_player_action(command_queue, credentials, message);
            }
            default:
                break;
        }
    }
}


void ClientHandler::run() {
    //    username = handle_login();
    username = generic_username;
    handle_map_names_request();
    const auto match_id = pick_match();
    sender = std::make_unique<Sender>(protocol, match_id.sender_queue);
    sender.value()->start();
    handle_game(match_id.command_queue, match_id.credentials);
    if (sender.has_value()) {
        sender.value()->join();
    }
}

void ClientHandler::stop() {
    Thread::stop();
    if (sender.has_value()) {
        sender.value()->stop();
        sender.value()->join();
        sender = std::nullopt;
    }
    protocol.kill();
}
