#include <iostream>

#include "common/catedra/socket.h"

#include "acceptor.h"

int main(const int argc, const char* argv[]) {
    if (argc != 2) {
        std::cerr << "Bad program call. Expected: " << argv[0] << " <port> <game parameters file>"
                  << std::endl;
        return EXIT_FAILURE;
    }
    Socket skt(argv[1]);
    Acceptor client_acceptor(std::move(skt));
    try {
        client_acceptor.run();
    } catch (const std::exception& e) {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unexpected exception: <unknown>" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
