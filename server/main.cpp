#include <iostream>

#include <yaml-cpp/yaml.h>

#include "common/catedra/socket.h"

#include "acceptor.h"
#define exit_char 'q'
int main(const int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Bad program call. Expected: " << argv[0] << " <port> <game parameters file>"
                  << std::endl;
        return EXIT_FAILURE;
    }
    char c;
    Socket skt(argv[1]);
    Acceptor client_acceptor(std::move(skt));
    client_acceptor.start();
    while (true) {
        std::cin >> c;
        if (c == exit_char) {
            break;
        }
    }
    client_acceptor.stop();
    client_acceptor.join();
    return EXIT_SUCCESS;
}
