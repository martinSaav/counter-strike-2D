#include <iostream>
#include <exception>
#include "ChatClient.h"

int main(int argc, char *argv[]) { try {
    int ret = -1;

    //Comprobamos que la cantidad de argumentos sea correcta
    if (argc != 3) {
        std::cerr << "Bad program call. Expected "
                  << argv[0]
                  << " without arguments.\n";
        return ret;
    }

    //Creamos el protocolo a usar del cliente
    ChatClient cliente(argv[1], argv[2]);

	cliente.run();
	
    ret = 0;
    return ret;
} catch (const std::exception& err) {
    std::cerr
        << "Something went wrong and an exception was caught: "
        << err.what()
        << "\n";
    return -1;
} catch (...) {
    std::cerr << "Something went wrong and an unknown exception was caught.\n";
    return -1;
}}
