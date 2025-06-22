#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <utility>
#include <vector>

#include "common/catedra/socket.h"
#include "common/catedra/thread.h"

#include "game_config.h"
#include "lobby.h"

class Acceptor: public Thread {
    Socket skt;
    GameConfig config;
    Lobby lobby;
    std::vector<Thread*> clients;

    void accept_clients();
    void reap_dead();
    void free_clients();

public:
    explicit Acceptor(Socket&& socket, GameConfig&& game_configuration):
            skt(std::move(socket)), config(std::move(game_configuration)), lobby(config) {}
    void run() override;
    void stop() override;
};

#endif  // ACCEPTOR_H
