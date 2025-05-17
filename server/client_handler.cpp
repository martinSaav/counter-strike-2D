//
// Created by matias on 15/05/25.
//

#include "client_handler.h"

// std::pair<Queue<MatchStatusDTO>, Player> ClientHandler::pick_match() {
//   while (true) {
//       auto message = protocol.recv_message();
//       message.
//    }
//}

void ClientHandler::receive_notification_about_sender_error() {
    if (sender.has_value()) {
        sender.value()->join();
        sender = std::nullopt;
    }
}


void ClientHandler::run() {}
