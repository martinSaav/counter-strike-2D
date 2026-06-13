#include "inputServerHandler.h"

#include <iostream>
#include <memory>
#include <utility>

InputServerHandler::InputServerHandler(Protocol& protocol, bool& gameOver):
        protocol(protocol), gameOver(gameOver) {}

InputServerHandler::~InputServerHandler() {}

void InputServerHandler::processEvents() {

    while (!gameOver) {

        const std::unique_ptr<Message> gameState = protocol.recv_message();

        const auto game = dynamic_cast<GameStateUpdate*>(gameState.get());

        if (game) {
            std::lock_guard<std::mutex> lock(mtx);
            messages.push(std::move(*game));

            if (game->is_game_ended()) {
                gameOver = true;
            }
        } else {
            std::cerr << "Unexpected exception: wrong message received \n";
        }
    }
}

std::optional<GameStateUpdate> InputServerHandler::getMessage() {
    std::lock_guard<std::mutex> lock(mtx);

    if (!messages.empty()) {
        GameStateUpdate message = messages.front();
        messages.pop();
        return message;
    }
    return std::nullopt;
}
