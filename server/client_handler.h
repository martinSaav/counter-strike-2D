//
// Created by matias on 15/05/25.
//

#ifndef SERVER_CLIENT_HANDLER_H_
#define SERVER_CLIENT_HANDLER_H_
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "common/catedra/queue.h"
#include "common/catedra/thread.h"
#include "common/protocol.h"
#include "server/lobby.h"
#include "server/sender.h"

#include "message_parser.h"

class ClientHandler: public Thread {
    Lobby& lobby;
    Socket skt;
    Protocol protocol;
    std::optional<std::unique_ptr<Sender>> sender;
    GameConfig& config;
    std::string username;
    std::string match_name;

    std::string handle_login();

    void handle_game_config_request();

    void handle_map_names_request();

    void handle_list_matches_request();


    GameIdentification handle_create_game_request(std::unique_ptr<Message>&& message);

    GameIdentification handle_join_game_request(std::unique_ptr<Message>&& message);

    std::optional<GameIdentification> pick_match();


    void handle_game(MessageParser parser, Queue<std::shared_ptr<PlayerCommand>>& command_queue,
                     const PlayerCredentials& credentials);

public:
    explicit ClientHandler(Lobby& lobby, Socket&& skt, GameConfig& config):
            lobby(lobby), skt(std::move(skt)), protocol(Protocol(this->skt)), config(config) {}

    void run() override;
    void stop() override;
};

#endif  // SERVER_CLIENT_HANDLER_H_
