#include "protocol.h"

#include <cstring>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../common/dto/buy_weapon_request.h"
#include "../common/dto/create_game_request.h"
#include "../common/dto/create_game_response.h"
#include "../common/dto/disconnect_request.h"
#include "../common/dto/game_list_request.h"
#include "../common/dto/game_list_response.h"
#include "../common/dto/game_ready_request.h"
#include "../common/dto/game_ready_response.h"
#include "../common/dto/game_state_update.h"
#include "../common/dto/join_game_request.h"
#include "../common/dto/join_game_response.h"
#include "../common/dto/join_team_request.h"
#include "../common/dto/join_team_response.h"
#include "../common/dto/login_request.h"
#include "../common/dto/map_names_request.h"
#include "../common/dto/map_names_response.h"
#include "../common/dto/player_action.h"
#include "../common/dto/select_skin_request.h"
#include "../common/dto/buy_ammo_request.h"
#include "../common/dto/game_config_info.h"


Protocol::Protocol(SocketInterface& peer): peer(peer) {}


std::unique_ptr<Message> Protocol::recv_message() {
    uint8_t header[HEADER_SIZE];
    peer.recvall(header, HEADER_SIZE);
    MessageType message_type = static_cast<MessageType>(header[0]);
    uint16_t payload_length;
    memcpy(&payload_length, header + 1, sizeof(payload_length));
    payload_length = ntohs(payload_length);
    std::vector<uint8_t> buffer(HEADER_SIZE + payload_length);

    buffer[0] = static_cast<uint8_t>(message_type);
    memcpy(buffer.data() + sizeof(message_type), &payload_length, sizeof(payload_length));


    if (payload_length > 0) {
        peer.recvall(buffer.data() + HEADER_SIZE, payload_length);
    }


    switch (message_type) {
        case MessageType::GameConfig: {
            return std::make_unique<GameConfigInfo>(
                    GameConfigInfo::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::LoginRequest: {
            return std::make_unique<LoginRequest>(
                    LoginRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::CreateGameRequest: {
            return std::make_unique<CreateGameRequest>(
                    CreateGameRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::JoinGameRequest: {
            return std::make_unique<JoinGameRequest>(
                    JoinGameRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::PlayerAction: {
            return std::make_unique<PlayerAction>(
                    PlayerAction::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::MapNamesRequest: {
            return std::make_unique<MapNamesRequest>(
                    MapNamesRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::GameListRequest: {
            return std::make_unique<GameListRequest>(
                    GameListRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::GameReadyRequest: {
            return std::make_unique<GameReadyRequest>(
                    GameReadyRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::JoinTeamRequest: {
            return std::make_unique<JoinTeamRequest>(
                    JoinTeamRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::SelectSkinRequest: {
            return std::make_unique<SelectSkinRequest>(
                    SelectSkinRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::BuyWeaponRequest: {
            return std::make_unique<BuyWeaponRequest>(
                    BuyWeaponRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::BuyAmmoRequest: {
            return std::make_unique<BuyAmmoRequest>(
                    BuyAmmoRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::DisconnectRequest: {
            return std::make_unique<DisconnectRequest>(
                    DisconnectRequest::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::CreateGameResponse: {
            return std::make_unique<CreateGameResponse>(
                    CreateGameResponse::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::JoinGameResponse: {
            return std::make_unique<JoinGameResponse>(
                    JoinGameResponse::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::GameStateUpdate: {
            return std::make_unique<GameStateUpdate>(
                    GameStateUpdate::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::MapNamesResponse: {
            return std::make_unique<MapNamesResponse>(
                    MapNamesResponse::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::GameListResponse: {
            return std::make_unique<GameListResponse>(
                    GameListResponse::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::GameReadyResponse: {
            return std::make_unique<GameReadyResponse>(
                    GameReadyResponse::deserialize(buffer.data(), buffer.size()));
        }
        case MessageType::JoinTeamResponse: {
            return std::make_unique<JoinTeamResponse>(
                    JoinTeamResponse::deserialize(buffer.data(), buffer.size()));
        }

        default:
            throw std::runtime_error("Unknown message type");
    }
}

void Protocol::send_message(const Message& message) {
    std::vector<uint8_t> buffer(message.serialized_size());
    message.serialize(buffer.data());
    peer.sendall(buffer.data(), buffer.size());
}

void Protocol::kill() {
    peer.shutdown(2);
    peer.close();
}
