#include <iostream>
#include <utility>

#include <yaml-cpp/yaml.h>

#include "common/catedra/socket.h"

#include "acceptor.h"
#include "configuration_loader.h"
#define exit_char 'q'
int main(const int argc, const char* argv[]) {
    if (argc != 3) {
        std::cerr << "Bad program call. Expected: " << argv[0] << " <port> <game parameters file>"
                  << std::endl;
        return EXIT_FAILURE;
    }
    char c;
    const ConfigurationLoader config_loader(argv[2]);
    GameConfig config = config_loader.load_configuration();
    Socket skt(argv[1]);
    Acceptor client_acceptor(std::move(skt), std::move(config));
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
