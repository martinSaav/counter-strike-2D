#include <QApplication>
#include <exception>
#include <iostream>

#include "chatClient.h"
#include "statistics.h"
#include "lobbywindow.h"

int main(int argc, char* argv[]) {
    // Comprobamos que la cantidad de argumentos sea correcta
    if (argc != 3) {
        const int ret = -1;
        std::cerr << "Bad program call. Expected " << argv[0] << " without arguments.\n";
        return ret;
    }

    std::string namePlayer;
    std::unique_ptr<GameStateUpdate> statistics;

    QApplication a(argc, argv);

    Socket socket(argv[1], argv[2]);
    Protocol protocol(socket);
    MainWindow lobby(protocol, namePlayer);

    lobby.move(900, 900);

    lobby.show();

    int result = a.exec();

    if (result == EXITAPP) {
        std::cerr << " QApplication closed." << std::endl;
        return result;

    } else if (result == EXITLOBBY) {

        // Creamos el juego para el cliente
        ChatClient client(protocol, namePlayer);

        result = client.run(statistics);

        if (result == CONTINUE) {
            Statistics window(statistics);
            window.exec();  // bloquea hasta que se cierre
        }
    }
    return 0;
}
