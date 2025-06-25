#include "inputServerHandler.h"

#include <iostream>
#include <memory>
#include <utility>

InputServerHandler::InputServerHandler(Protocol& protocolo, bool& gameOver):
        protocolo(protocolo), gameOver(gameOver) {}

InputServerHandler::~InputServerHandler() {}

void InputServerHandler::processEvents() {

    while (!gameOver) {

        const std::unique_ptr<Message> gameState = protocolo.recv_message();

        const auto game = dynamic_cast<GameStateUpdate*>(gameState.get());

        if (game) {
            std::lock_guard<std::mutex> lock(mtx);
            mensajes.push(std::move(*game));

            if (game->is_game_ended()) {
                gameOver = true;
            }
        } else {
            std::cerr << "Unexpected exception: wrong message recibed \n";
        }
    }
}

std::optional<GameStateUpdate> InputServerHandler::getMensaje() {
    std::lock_guard<std::mutex> lock(mtx);

    if (!mensajes.empty()) {
        GameStateUpdate mensaje = mensajes.front();
        mensajes.pop();
        return mensaje;
    }
    return std::nullopt;
}
