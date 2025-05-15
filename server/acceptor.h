#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <utility>
#include <vector>

#include "common/catedra/socket.h"
#include "common/catedra/thread.h"

#include "lobby.h"

class Acceptor {
    Socket skt;
    Lobby lobby;
    std::vector<Thread*> clients;

    void accept_clients();

public:
    explicit Acceptor(Socket&& socket): skt(std::move(socket)) {}
    void run();
};

#endif  // ACCEPTOR_H
