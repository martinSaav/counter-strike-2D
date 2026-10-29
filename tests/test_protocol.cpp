#include <cstring>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "common/action.h"
#include "common/dto/buy_ammo_request.h"
#include "common/dto/buy_weapon_request.h"
#include "common/dto/create_game_request.h"
#include "common/dto/game_config_info.h"
#include "common/dto/game_list_response.h"
#include "common/dto/game_state_update.h"
#include "common/dto/join_game_request.h"
#include "common/dto/join_team_request.h"
#include "common/dto/login_request.h"
#include "common/dto/map_names_response.h"
#include "common/dto/player_action.h"
#include "common/dto/select_skin_request.h"
#include "common/dto/simple_messages.h"
#include "common/game_info.h"
#include "common/protocol.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "mock_socket.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;

namespace {

using Bytes = std::vector<uint8_t>;

// Conecta un MockSocket a un buffer en memoria: lo que se manda se acumula en `bytes` y lo que
// se recibe sale de ahi en orden. Si se pide mas de lo que queda, recvall devuelve 0 (como un
// socket cerrado).
class Wire {
public:
    Bytes bytes;
    std::size_t read_pos = 0;
    NiceMock<MockSocket> socket;

    explicit Wire(Bytes initial = {}): bytes(std::move(initial)) {
        ON_CALL(socket, sendall(_, _))
                .WillByDefault(Invoke([this](const void* data, unsigned int size) {
                    const auto* first = static_cast<const uint8_t*>(data);
                    bytes.insert(bytes.end(), first, first + size);
                    return static_cast<int>(size);
                }));
        ON_CALL(socket, recvall(_, _)).WillByDefault(Invoke([this](void* dst, unsigned int size) {
            if (read_pos + size > bytes.size()) {
                return 0;
            }
            std::memcpy(dst, bytes.data() + read_pos, size);
            read_pos += size;
            return static_cast<int>(size);
        }));
    }
};

// Los bytes exactos que pone Protocol en el socket al mandar `message`
Bytes bytes_of(const Message& message) {
    Wire wire;
    Protocol protocol(wire.socket);
    protocol.send_message(message);
    return wire.bytes;
}

// Manda el mensaje y lo vuelve a leer con el mismo Protocol, como haria el otro extremo
template <typename M>
M round_trip(const M& message) {
    Wire wire;
    Protocol protocol(wire.socket);
    protocol.send_message(message);
    const std::unique_ptr<Message> received = protocol.recv_message();
    EXPECT_EQ(wire.read_pos, wire.bytes.size()) << "quedaron bytes sin leer";
    const auto* typed = dynamic_cast<const M*>(received.get());
    if (typed == nullptr) {
        throw std::runtime_error("se recibio un mensaje de otro tipo");
    }
    return *typed;
}

// Lee un mensaje a partir de bytes armados a mano
std::unique_ptr<Message> receive(Bytes bytes) {
    Wire wire(std::move(bytes));
    Protocol protocol(wire.socket);
    return protocol.recv_message();
}

PlayerInfo sample_player() {
    return PlayerInfo("p", 0x0102, 0x0304, 100, Status::Alive, 800, 2, 1, Action::Idle, 10, 11, "s",
                      Weapon::AK47, 30, Weapon::Glock, 20, Weapon::Knife, Weapon::None,
                      Weapon::AK47, 30, 50);
}

GunConfigInfo sample_gun(int32_t price) {
    return GunConfigInfo(30, 90, 10, 40, price, 3, 2, 50, 12.5f);
}

GameConfigInfo sample_config() {
    const SiteInfo ct_site(1, 2, 3, 4, {{5, 6}, {7, 8}});
    const SiteInfo tt_site(9, 10, 11, 12, {{13, 14}});
    MapConfigInfo map("desierto", 40, 30, {StructureInfo(1, 2, 3, 4), StructureInfo(5, 6, 7, 8)},
                      {BombSiteInfo(2, 3, 20, 21)}, ct_site, tt_site, "aztec");
    return GameConfigInfo(100, 10, 800, 5, 5, 40, sample_gun(0), sample_gun(400), sample_gun(2700),
                          sample_gun(4750), sample_gun(1700), 5, 3, 100, 3000, 1500, 20, 40, 5, 300,
                          1, 30, std::move(map), 90, 60);
}

}  // namespace

// ---- Formato exacto en el socket: encabezado (tipo, largo u16) y campos en orden de red ----

TEST(ProtocolWireFormatTest, LoginRequest) {
    const Bytes expected = {0x01, 0x00, 0x04, 0x00, 0x02, 'a', 'b'};
    EXPECT_EQ(bytes_of(LoginRequest("ab")), expected);
}

TEST(ProtocolWireFormatTest, EmptyRequestIsJustTheHeader) {
    const Bytes expected = {0x03, 0x00, 0x00};
    EXPECT_EQ(bytes_of(GameListRequest()), expected);
}

TEST(ProtocolWireFormatTest, SuccessResponseIsOneByte) {
    const Bytes expected = {0x63, 0x00, 0x01, 0x01};
    EXPECT_EQ(bytes_of(JoinGameResponse(true)), expected);
}

TEST(ProtocolWireFormatTest, PlayerAction) {
    const Bytes expected = {0x0B, 0x00, 0x05, 0x05, 0x01, 0x02, 0x03, 0x04};
    EXPECT_EQ(bytes_of(PlayerAction(Action::Shoot, 0x0102, 0x0304)), expected);
}

TEST(ProtocolWireFormatTest, PlayerInfoUsesNetworkOrder) {
    ByteWriter writer;
    sample_player().write(writer);

    const Bytes expected = {
            0x00, 0x01, 'p',   // nombre
            0x01,              // vivo
            0x01, 0x02,        // pos_x
            0x03, 0x04,        // pos_y
            0x00, 0x64,        // vida 100
            0x03, 0x20,        // plata 800
            0x00, 0x02,        // kills
            0x00, 0x01,        // muertes
            0x0E,              // accion Idle
            0x00, 0x0A,        // mira x
            0x00, 0x0B,        // mira y
            0x00, 0x01, 's',   // skin
            0x04, 0x00, 0x1E,  // primaria AK47 con 30 balas
            0x03, 0x00, 0x14,  // secundaria Glock con 20 balas
            0x01,              // cuchillo
            0x00,              // sin bomba
            0x04, 0x00, 0x1E,  // arma activa AK47 con 30 balas
            0x32,              // avance de la tarea 50
    };
    EXPECT_EQ(writer.data(), expected);
}

TEST(ProtocolWireFormatTest, GameStateUpdateFloatsUseNetworkOrder) {
    const GameStateUpdate state(true, false, 1, 1.0f, true, false, false, 0, 0, 0.0f,
                                Team::Terrorists, Team::Terrorists, {}, {});
    const Bytes bytes = bytes_of(state);

    // encabezado (3) + game_started, game_ended, round (3) y luego round_time = 1.0f
    const Bytes round_time(bytes.begin() + 6, bytes.begin() + 10);
    EXPECT_EQ(round_time, (Bytes{0x3F, 0x80, 0x00, 0x00}));
}

// ---- Ida y vuelta: lo que se manda es lo que se recibe, para cada mensaje ----

TEST(ProtocolRoundTripTest, LoginRequest) {
    EXPECT_EQ(round_trip(LoginRequest("martin")).get_username(), "martin");
}

TEST(ProtocolRoundTripTest, JoinGameRequest) {
    EXPECT_EQ(round_trip(JoinGameRequest("partida")).get_game_name(), "partida");
}

TEST(ProtocolRoundTripTest, SelectSkinRequest) {
    EXPECT_EQ(round_trip(SelectSkinRequest("seal")).get_skin(), "seal");
}

TEST(ProtocolRoundTripTest, CreateGameRequest) {
    const CreateGameRequest received = round_trip(CreateGameRequest("partida", "desierto"));
    EXPECT_EQ(received.get_game_name(), "partida");
    EXPECT_EQ(received.get_map_name(), "desierto");
}

TEST(ProtocolRoundTripTest, JoinTeamRequest) {
    EXPECT_EQ(round_trip(JoinTeamRequest(Team::CounterTerrorists)).get_team(),
              Team::CounterTerrorists);
}

TEST(ProtocolRoundTripTest, BuyWeaponRequest) {
    EXPECT_EQ(round_trip(BuyWeaponRequest(Weapon::AWP)).get_weapon(), Weapon::AWP);
}

TEST(ProtocolRoundTripTest, BuyAmmoRequest) {
    EXPECT_EQ(round_trip(BuyAmmoRequest(WeaponType::Secondary)).get_weapon_type(),
              WeaponType::Secondary);
}

TEST(ProtocolRoundTripTest, PlayerAction) {
    const PlayerAction received = round_trip(PlayerAction(Action::PlantBomb, 640, 65535));
    EXPECT_EQ(received.get_action(), Action::PlantBomb);
    EXPECT_EQ(received.get_pos_mouse_x(), 640);
    EXPECT_EQ(received.get_pos_mouse_y(), 65535);
}

TEST(ProtocolRoundTripTest, EmptyRequests) {
    EXPECT_EQ(round_trip(MapNamesRequest()).type(), MessageType::MapNamesRequest);
    EXPECT_EQ(round_trip(GameListRequest()).type(), MessageType::GameListRequest);
    EXPECT_EQ(round_trip(GameReadyRequest()).type(), MessageType::GameReadyRequest);
    EXPECT_EQ(round_trip(DisconnectRequest()).type(), MessageType::DisconnectRequest);
}

TEST(ProtocolRoundTripTest, SuccessResponses) {
    EXPECT_TRUE(round_trip(LoginResponse(true)).get_success());
    EXPECT_FALSE(round_trip(CreateGameResponse(false)).get_success());
    EXPECT_TRUE(round_trip(JoinGameResponse(true)).get_success());
    EXPECT_FALSE(round_trip(JoinTeamResponse(false)).get_success());
    EXPECT_TRUE(round_trip(GameReadyResponse(true)).get_success());
}

TEST(ProtocolRoundTripTest, MapNamesResponse) {
    const std::list<std::string> names = {"desierto", "pueblo-azteca", ""};
    EXPECT_EQ(round_trip(MapNamesResponse(names)).get_mapGames(), names);
}

TEST(ProtocolRoundTripTest, GameListResponse) {
    const std::list<GameInfo> games = {{"uno", "desierto", 1, 10}, {"dos", "entrenamiento", 4, 4}};
    const std::list<GameInfo> received = round_trip(GameListResponse(games)).get_games();

    ASSERT_EQ(received.size(), 2u);
    EXPECT_EQ(received.back().name, "dos");
    EXPECT_EQ(received.back().map_name, "entrenamiento");
    EXPECT_EQ(received.back().current_players, 4);
    EXPECT_EQ(received.back().max_players, 4);
}

TEST(ProtocolRoundTripTest, EmptyGameListResponse) {
    EXPECT_TRUE(round_trip(GameListResponse({})).get_games().empty());
}

TEST(ProtocolRoundTripTest, GameStateUpdate) {
    const GameStateUpdate state(true, false, 3, 12.5f, true, false, true, 300, 400, 31.75f,
                                Team::CounterTerrorists, Team::Terrorists, {sample_player()},
                                {DroppedWeapon(Weapon::M3, 7, 8)}, RoundStage::PostPlant, 97.25f);
    const GameStateUpdate received = round_trip(state);

    EXPECT_TRUE(received.is_game_started());
    EXPECT_FALSE(received.is_game_ended());
    EXPECT_EQ(received.get_round(), 3);
    EXPECT_FLOAT_EQ(received.get_round_time(), 12.5f);
    EXPECT_TRUE(received.is_bomb_planted());
    EXPECT_EQ(received.get_bomb_x(), 300);
    EXPECT_EQ(received.get_bomb_y(), 400);
    EXPECT_FLOAT_EQ(received.get_bomb_timer(), 31.75f);
    EXPECT_EQ(received.get_round_winner(), Team::CounterTerrorists);
    EXPECT_EQ(received.get_game_winner(), Team::Terrorists);
    EXPECT_EQ(received.get_stage(), RoundStage::PostPlant);
    EXPECT_FLOAT_EQ(received.get_time_left(), 97.25f);

    ASSERT_EQ(received.get_players().size(), 1u);
    const PlayerInfo player = received.get_players().front();
    EXPECT_EQ(player.get_user_name(), "p");
    EXPECT_EQ(player.get_pos_x(), 0x0102);
    EXPECT_EQ(player.get_pos_y(), 0x0304);
    EXPECT_EQ(player.get_health(), 100);
    EXPECT_EQ(player.get_money(), 800);
    EXPECT_EQ(player.get_kills(), 2);
    EXPECT_EQ(player.get_deaths(), 1);
    EXPECT_EQ(player.get_pos_shoot_x(), 10);
    EXPECT_EQ(player.get_skin(), "s");
    EXPECT_EQ(player.get_secondary_weapon(), Weapon::Glock);
    EXPECT_EQ(player.get_active_weapon_ammo(), 30);
    EXPECT_EQ(player.get_task_progress(), 50);

    ASSERT_EQ(received.get_dropped_weapons().size(), 1u);
    const DroppedWeapon weapon = received.get_dropped_weapons().front();
    EXPECT_EQ(weapon.get_weapon(), Weapon::M3);
    EXPECT_EQ(weapon.get_pos_x(), 7);
    EXPECT_EQ(weapon.get_pos_y(), 8);
}

TEST(ProtocolRoundTripTest, GameConfigInfo) {
    const GameConfigInfo received = round_trip(sample_config());

    EXPECT_EQ(received.get_player_health(), 100);
    EXPECT_EQ(received.get_ammo_price(), 40);
    EXPECT_EQ(received.get_awp_config().get_gun_price(), 4750);
    EXPECT_FLOAT_EQ(received.get_m3_config().get_angle(), 12.5f);
    EXPECT_EQ(received.get_defuse_time(), 5);
    EXPECT_EQ(received.get_game_rate(), 30);
    EXPECT_EQ(received.get_cone_angle(), 90);
    EXPECT_EQ(received.get_opacity(), 60);

    const MapConfigInfo& map = received.get_map_config();
    EXPECT_EQ(map.get_map_name(), "desierto");
    EXPECT_EQ(map.get_map_width(), 40);
    EXPECT_EQ(map.get_theme(), "aztec");
    ASSERT_EQ(map.get_structures().size(), 2u);
    EXPECT_EQ(map.get_structures().back().get_x(), 7);
    ASSERT_EQ(map.get_bombsites().size(), 1u);
    EXPECT_EQ(map.get_bombsites().front().get_y(), 21);
    EXPECT_EQ(map.get_ct_site().get_spawns().size(), 2u);
    EXPECT_EQ(map.get_tt_site().get_spawns().front(), std::make_pair(13, 14));
}

// ---- Mensajes mal formados: se rechazan con ProtocolError, sin leer fuera del buffer ----

TEST(ProtocolMalformedTest, UnknownTypeThrows) {
    EXPECT_THROW(receive({0xFF, 0x00, 0x00}), ProtocolError);
}

TEST(ProtocolMalformedTest, TruncatedPayloadThrows) {
    // PlayerAction necesita 5 bytes y el encabezado anuncia solo 1
    EXPECT_THROW(receive({0x0B, 0x00, 0x01, 0x05}), ProtocolError);
}

TEST(ProtocolMalformedTest, StringLongerThanPayloadThrows) {
    // El nombre dice tener 10 bytes pero el payload termina despues del primero
    EXPECT_THROW(receive({0x01, 0x00, 0x03, 0x00, 0x0A, 'a'}), ProtocolError);
}

TEST(ProtocolMalformedTest, TrailingBytesThrow) {
    EXPECT_THROW(receive({0x63, 0x00, 0x02, 0x01, 0x00}), ProtocolError);
}

TEST(ProtocolMalformedTest, PayloadTooLargeToSendThrows) {
    // Dos nombres de 40000 bytes no entran en el largo u16 del encabezado
    const std::string big(40000, 'x');
    Wire wire;
    Protocol protocol(wire.socket);
    EXPECT_THROW(protocol.send_message(MapNamesResponse({big, big})), ProtocolError);
    EXPECT_TRUE(wire.bytes.empty());
}

// ---- Conexion cerrada ----

TEST(ProtocolConnectionTest, ThrowsConnectionClosedWhenHeaderNotReceived) {
    MockSocket mock_socket;
    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Return(0));

    Protocol protocol(mock_socket);
    EXPECT_THROW(protocol.recv_message(), ConnectionClosed);
}

TEST(ProtocolConnectionTest, ThrowsConnectionClosedWhenPayloadNotReceived) {
    // El encabezado anuncia 6 bytes de payload que nunca llegan
    EXPECT_THROW(receive({0x01, 0x00, 0x06}), ConnectionClosed);
}
