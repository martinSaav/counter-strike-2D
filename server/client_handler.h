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

class ClientHandler: public Thread {
    Lobby& lobby;
    Socket skt;
    Protocol protocol;
    std::optional<std::unique_ptr<Sender>> sender;
    std::string username;
    std::string match_name;

    std::string handle_login();

    void handle_map_names_request();

    void handle_list_matches_request();

    void handle_disconnect_request(Queue<PlayerCommand>& command_queue,
                                   const PlayerCredentials& credentials);

    GameIdentification handle_create_game_request(std::unique_ptr<Message>&& message);

    GameIdentification handle_join_game_request(std::unique_ptr<Message>&& message);

    std::optional<GameIdentification> pick_match();

    static CommandType cast_action_to_command(Action action);

    static void handle_player_action(Queue<PlayerCommand>& command_queue,
                                     const PlayerCredentials& credentials,
                                     const std::unique_ptr<Message>& message);

    static void handle_game_ready(Queue<PlayerCommand>& command_queue,
                                  const PlayerCredentials& credentials);


    static void handle_change_skin(Queue<PlayerCommand>& command_queue,
                                   const PlayerCredentials& credentials,
                                   const std::unique_ptr<Message>& message);

    void handle_game(Queue<PlayerCommand>& command_queue, const PlayerCredentials& credentials);

public:
    explicit ClientHandler(Lobby& lobby, Socket&& skt):
            lobby(lobby), skt(std::move(skt)), protocol(Protocol(this->skt)) {}

    void run() override;
    void stop() override;
};

#endif  // SERVER_CLIENT_HANDLER_H_
