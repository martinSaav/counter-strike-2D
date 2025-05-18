//
// Created by matias on 15/05/25.
//

#include "client_handler.h"

#include "common/dto/login_request.h"
#include "common/message.h"

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


GameIdentification ClientHandler::pick_match() {
    while (true) {
        auto message = protocol.recv_message();
        switch (message->type()) {
            case MessageType::GameListRequest: {
            }
            case MessageType::CreateGameRequest: {
            }
            case MessageType::JoinGameRequest: {
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


void ClientHandler::run() { username = handle_login(); }

void ClientHandler::stop() {
    Thread::stop();
    if (sender.has_value()) {
        sender.value()->stop();
        sender.value()->join();
        sender = std::nullopt;
    }
    protocol.kill();
}
