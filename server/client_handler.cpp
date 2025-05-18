//
// Created by matias on 15/05/25.
//

#include "client_handler.h"

#include <list>
#include <vector>

#include "common/dto/login_request.h"
#include "common/game.h"
#include "common/message.h"
#define generic_match_name "match"
#include "common/dto/game_list_response.h"

std::string ClientHandler::handle_login() {
    while (true) {
        const auto message = protocol.recv_message();
        switch (message->type()) {
            case MessageType::LoginRequest: {
                const auto login_message = static_cast<LoginRequest*>(message.get());
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

GameIdentification ClientHandler::handle_create_game_request() {
    return lobby.create_match(generic_match_name, username);
}

GameIdentification ClientHandler::handle_join_game_request() {
    return lobby.join_match(generic_match_name, username);
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
                return handle_create_game_request();
            }
            case MessageType::JoinGameRequest: {
                return handle_join_game_request();
            }
            default:
                break;
        }
    }
}

void ClientHandler::receive_notification_about_sender_error() {
    if (sender.has_value()) {
        sender.value()->join();
        sender = std::nullopt;
    }
}

void ClientHandler::run() {
    username = handle_login();
    auto match_id = pick_match();
    sender = std::make_unique<Sender>(protocol, match_id.sender_queue, this);
    sender.value()->start();
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
