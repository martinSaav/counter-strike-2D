#include "server/acceptor.h"

#include <utility>

#include "catedra/liberror.h"
#include "server/client_handler.h"

#include "sender.h"

void Acceptor::run() {
    while (should_keep_running()) {
        try {
            accept_clients();
        } catch (const std::exception& err) {
            if (!should_keep_running()) {  // el hilo main hizo stop y cerro el socket, por eso
                // la excepcion
                break;
            }
            std::cerr << "Unexpected exception: " << err.what()
                      << std::endl;  // Ocurrio una excepcion inesperada
            break;
        } catch (...) {
            std::cerr << "Unexpected exception: <unknown>\n";  // Ocurrio un error desconocido
            break;
        }
    }
    free_clients();
}

void Acceptor::accept_clients() {
    Socket peer = skt.accept();
    Thread* c = new ClientHandler(lobby, std::move(peer), config);
    clients.push_back(c);
    c->start();
    reap_dead();
}


void Acceptor::stop() {
    Thread::stop();
    skt.shutdown(SHUT_RDWR);
}


void Acceptor::reap_dead() {
    auto clients_iterator = clients.begin();
    while (clients_iterator != clients.end()) {
        if (!(*clients_iterator)->is_alive()) {
            (*clients_iterator)->join();
            delete *clients_iterator;
            clients_iterator = clients.erase(clients_iterator);
        } else {
            ++clients_iterator;
        }
    }
    lobby.remove_finished_matches();
}


void Acceptor::free_clients() {
    for (const auto& client: clients) {
        if (client) {
            if (client->is_alive()) {
                client->stop();
            }
            client->join();
            delete client;
        }
    }
    lobby.kill_all_matches();
}
