#include <QApplication>
#include <iostream>
#include <exception>
#include "lobbywindow.h"
#include "ChatClient.h"

int main(int argc, char *argv[])
{
    int ret = -1;

    //Comprobamos que la cantidad de argumentos sea correcta
    if (argc != 3) {
        std::cerr << "Bad program call. Expected "
                  << argv[0]
                  << " without arguments.\n";
        return ret;
    }
    
    QApplication a(argc, argv);

    Socket socket(argv[1], argv[2]);
    Protocol protocolo(socket);
    MainWindow lobby(protocolo);

    lobby.move(900, 900);

    lobby.show();

    int result = a.exec();

    if (result == EXITAPP) {
        std::cerr << " QApplication closed." << std::endl;
        return result;
    }

    //Creamos el protocolo a usar del cliente
    ChatClient cliente(protocolo);

    cliente.run();

    return 0;
}
