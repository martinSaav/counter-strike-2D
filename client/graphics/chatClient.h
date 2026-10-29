#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#include <memory>
#include <optional>
#include <string>

#include "../../common/dto/game_config_info.h"

#include "render.h"

class InputHandler;
class InputServerHandler;

// Valores de retorno de ChatClient::run
const int CONTINUE = 1;
const int CLIENTCLOSED = 2;

// La partida del lado del cliente: abre la ventana, corre el loop de juego y al terminar deja
// el ultimo estado para la pantalla de estadisticas.
class ChatClient {
private:
    Protocol& protocol;
    std::string& namePlayer;
    bool clientClosed = false;
    bool gameOver = false;

    // Devuelve el ultimo estado que dibujo
    std::optional<GameStateUpdate> gameLoop(Render& render, InputHandler& inputHandler,
                                            InputServerHandler& inputServerHandler);
    int collectStatistics(InputServerHandler& inputServerHandler,
                          std::optional<GameStateUpdate> lastState,
                          std::unique_ptr<GameStateUpdate>& statistics);

public:
    ChatClient(Protocol& protocol, std::string& namePlayer);

    // CONTINUE si la partida termino y hay estadisticas; CLIENTCLOSED si el jugador salio o se
    // corto la conexion
    int run(std::unique_ptr<GameStateUpdate>& statistics);
};
#endif
