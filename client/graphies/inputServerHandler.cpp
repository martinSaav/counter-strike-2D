#include "inputServerHandler.h"

#include <memory>
InputServerHandler::InputServerHandler(Protocol& protocolo): protocolo(protocolo) {}

InputServerHandler::~InputServerHandler() {}

void InputServerHandler::processEvents() {

    while (!exitGame()) {

        const std::unique_ptr<Message> gameState = protocolo.recv_message();

        const auto game = dynamic_cast<GameStateUpdate*>(gameState.get());

        std::lock_guard<std::mutex> lock(mtx);
        mensajes.push(*game);
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

bool InputServerHandler::exitGame() { return quit; }
