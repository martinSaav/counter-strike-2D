#include "message_registry.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

#include "dto/buy_ammo_request.h"
#include "dto/buy_weapon_request.h"
#include "dto/create_game_request.h"
#include "dto/game_config_info.h"
#include "dto/game_list_response.h"
#include "dto/game_state_update.h"
#include "dto/join_game_request.h"
#include "dto/join_team_request.h"
#include "dto/login_request.h"
#include "dto/map_names_response.h"
#include "dto/player_action.h"
#include "dto/select_skin_request.h"
#include "dto/simple_messages.h"

namespace {

using MessageReader = std::function<std::unique_ptr<Message>(ByteReader&)>;

template <typename M>
std::pair<const MessageType, MessageReader> entry() {
    return {M::TYPE,
            [](ByteReader& reader) { return std::make_unique<M>(M::read_payload(reader)); }};
}

// Un mensaje nuevo se suma aca con una linea; Protocol no cambia.
const std::unordered_map<MessageType, MessageReader>& readers() {
    static const std::unordered_map<MessageType, MessageReader> table = {
            // Cliente -> servidor
            entry<LoginRequest>(),
            entry<MapNamesRequest>(),
            entry<GameListRequest>(),
            entry<JoinGameRequest>(),
            entry<CreateGameRequest>(),
            entry<JoinTeamRequest>(),
            entry<SelectSkinRequest>(),
            entry<GameReadyRequest>(),
            entry<BuyWeaponRequest>(),
            entry<BuyAmmoRequest>(),
            entry<PlayerAction>(),
            entry<DisconnectRequest>(),
            // Servidor -> cliente
            entry<GameConfigInfo>(),
            entry<LoginResponse>(),
            entry<MapNamesResponse>(),
            entry<GameListResponse>(),
            entry<JoinGameResponse>(),
            entry<CreateGameResponse>(),
            entry<JoinTeamResponse>(),
            entry<GameReadyResponse>(),
            entry<GameStateUpdate>(),
    };
    return table;
}

}  // namespace

std::unique_ptr<Message> read_message(MessageType type, ByteReader& payload) {
    const auto& table = readers();
    const auto reader = table.find(type);
    if (reader == table.end()) {
        throw ProtocolError("tipo de mensaje desconocido: " +
                            std::to_string(static_cast<int>(type)));
    }
    std::unique_ptr<Message> message = reader->second(payload);
    if (payload.remaining() != 0) {
        throw ProtocolError("sobran " + std::to_string(payload.remaining()) +
                            " bytes en el mensaje de tipo " +
                            std::to_string(static_cast<int>(type)));
    }
    return message;
}
