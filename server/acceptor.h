#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <utility>
#include <vector>

#include "common/catedra/socket.h"
#include "common/catedra/thread.h"

#include "lobby.h"

class Acceptor: public Thread {
    Socket skt;
    Lobby lobby;
    std::vector<Thread*> clients;

    void accept_clients();
    void reap_dead();
    void free_clients();

public:
    explicit Acceptor(Socket&& socket): skt(std::move(socket)) {}
    void run() override;
    void stop() override;
};

#endif  // ACCEPTOR_H
