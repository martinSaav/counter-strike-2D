#include <QApplication>
#include <exception>
#include <iostream>

#include "chatClient.h"
#include "lobbywindow.h"
#include "estadistics.h"

int main(int argc, char* argv[]) {
    // Comprobamos que la cantidad de argumentos sea correcta
    if (argc != 3) {
        const int ret = -1;
        std::cerr << "Bad program call. Expected " << argv[0] << " without arguments.\n";
        return ret;
    }

    std::string namePlayer;
    std::unique_ptr<GameStateUpdate> estadistics;

    QApplication a(argc, argv);

    Socket socket(argv[1], argv[2]);
    Protocol protocolo(socket);
    MainWindow lobby(protocolo, namePlayer);

    lobby.move(900, 900);

    lobby.show();

    int result = a.exec();

    if (result == EXITAPP) {
        std::cerr << " QApplication closed." << std::endl;
        return result;

    } else if (result == EXITLOBBY) {

        // Creamos el juego para el cliente
        ChatClient cliente(protocolo, namePlayer);

        result = cliente.run(estadistics);

        if (result == CONTINUAR){
            Estadistics window(estadistics);
            window.exec(); //bloquea hasta que se cierre
        }
    }
    return 0;
}
