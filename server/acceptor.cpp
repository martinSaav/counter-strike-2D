#include "server/acceptor.h"

#include <utility>

#include "server/client_handler.h"

#include "sender.h"
void Acceptor::run() {
    while (true) {
        try {
            accept_clients();
        } catch (const std::exception& err) {
            std::cerr << "Unexpected exception: " << err.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";
            break;
        }
    }
}

void Acceptor::accept_clients() {
    Socket peer = skt.accept();
    Protocol protocol(peer);
    Thread* c = new ClientHandler(std::move(protocol), lobby, std::move(peer));
    clients.push_back(c);
    c->start();
}
