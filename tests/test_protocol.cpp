#include "common/action.h"
#include "common/dto/create_game_request.h"
#include "common/dto/create_game_response.h"
#include "common/dto/game_list_request.h"
#include "common/dto/game_list_response.h"
#include "common/dto/join_game_request.h"
#include "common/dto/join_game_response.h"
#include "common/dto/login_request.h"
#include "common/dto/player_action.h"
#include "common/game_info.h"
#include "common/protocol.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "mock_socket.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

// LoginRequest Test Cases

TEST(ProtocolTest, SendsLoginRequestCorrectly) {
    MockSocket mock_socket;

    const std::string username = "martin";
    const LoginRequest req(username);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::LoginRequest);
    const uint16_t len = htons(username.size());
    memcpy(&serialized[1], &len, sizeof(len));
    memcpy(&serialized[3], username.c_str(), username.size());

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesLoginRequestCorrectly) {
    MockSocket mock_socket;

    const std::string username = "martin";
    LoginRequest req(username);
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::LoginRequest);
    const uint16_t len = htons(username.size());
    memcpy(&serialized[1], &len, sizeof(len));
    memcpy(&serialized[3], username.c_str(), username.size());

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    EXPECT_CALL(mock_socket, recvall(_, username.size()))
            .WillOnce(Invoke([&](void* dst, unsigned int) {
                memcpy(dst, serialized.data() + 3, username.size());
                return username.size();
            }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* login = dynamic_cast<LoginRequest*>(msg.get());
    ASSERT_NE(login, nullptr);
    EXPECT_EQ(login->get_username(), username);
}

// CreateGameRequest Test Cases


TEST(ProtocolTest, SendsCreateGameCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const std::string map_name = "a map";
    const CreateGameRequest req(game_name, map_name);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    size_t offset = 3;
    serialized[0] = static_cast<uint8_t>(MessageType::CreateGameRequest);
    uint16_t game_name_length = htons(static_cast<uint16_t>(game_name.size()));
    memcpy(&serialized[offset], &game_name_length, sizeof(game_name_length));
    offset += sizeof(game_name_length);
    memcpy(&serialized[offset], game_name.c_str(), game_name.size());
    offset += game_name.size();
    uint16_t map_name_length = htons(static_cast<uint16_t>(map_name.size()));
    memcpy(&serialized[offset], &map_name_length, sizeof(map_name_length));
    offset += sizeof(map_name_length);
    memcpy(&serialized[offset], map_name.c_str(), map_name.size());
    offset += map_name.size();
    const uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
    memcpy(&serialized[1], &payload_length, sizeof(payload_length));


    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}
TEST(ProtocolTest, ReceivesCreateGameCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const std::string map_name = "a map";
    const CreateGameRequest req(game_name, map_name);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    size_t offset = 3;
    serialized[0] = static_cast<uint8_t>(MessageType::CreateGameRequest);
    uint16_t game_name_length = htons(static_cast<uint16_t>(game_name.size()));
    memcpy(&serialized[offset], &game_name_length, sizeof(game_name_length));
    offset += sizeof(game_name_length);
    memcpy(&serialized[offset], game_name.c_str(), game_name.size());
    offset += game_name.size();
    uint16_t map_name_length = htons(static_cast<uint16_t>(map_name.size()));
    memcpy(&serialized[offset], &map_name_length, sizeof(map_name_length));
    offset += sizeof(map_name_length);
    memcpy(&serialized[offset], map_name.c_str(), map_name.size());
    offset += map_name.size();
    const uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
    memcpy(&serialized[1], &payload_length, sizeof(payload_length));

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    size_t payload_size = serialized_size - 3;
    EXPECT_CALL(mock_socket, recvall(_, payload_size))
        .WillOnce(Invoke([&](void* dst, unsigned int) {
            memcpy(dst, serialized.data() + 3, payload_size);
            return payload_size;
        }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* create_game = dynamic_cast<CreateGameRequest*>(msg.get());
    ASSERT_NE(create_game, nullptr);
    EXPECT_EQ(create_game->get_game_name(), game_name);
    EXPECT_EQ(create_game->get_map_name(), map_name);
}


// CreateGameResponse Test Cases

TEST(ProtocolTest, SendsCreateGameResponseCorrectly) {
    MockSocket mock_socket;

    const bool success = true;
    const CreateGameResponse req(success);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::CreateGameResponse);
    const uint16_t len = htons(1);
    memcpy(&serialized[1], &len, sizeof(len));
    serialized[3] = static_cast<uint8_t>(success);

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesCreateGameResponseCorrectly) {
    MockSocket mock_socket;

    const bool success = true;
    const CreateGameResponse req(success);
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::CreateGameResponse);
    const uint16_t len = htons(1);
    memcpy(&serialized[1], &len, sizeof(len));
    serialized[3] = static_cast<uint8_t>(success);

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    EXPECT_CALL(mock_socket, recvall(_, 1)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data() + 3, 1);
        return 1;
    }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* create_game_resp = dynamic_cast<CreateGameResponse*>(msg.get());
    ASSERT_NE(create_game_resp, nullptr);
    EXPECT_EQ(create_game_resp->get_success(), success);
}

// JoinGameRequest Test Cases

TEST(ProtocolTest, SendsJoinGameRequestCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const JoinGameRequest req(game_name);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::JoinGameRequest);
    const uint16_t len = htons(game_name.size());
    memcpy(&serialized[1], &len, sizeof(len));
    memcpy(&serialized[3], game_name.c_str(), game_name.size());

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesJoinGameRequestCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const JoinGameRequest req(game_name);
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::JoinGameRequest);
    const uint16_t len = htons(game_name.size());
    memcpy(&serialized[1], &len, sizeof(len));
    memcpy(&serialized[3], game_name.c_str(), game_name.size());

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    EXPECT_CALL(mock_socket, recvall(_, game_name.size()))
            .WillOnce(Invoke([&](void* dst, unsigned int) {
                memcpy(dst, serialized.data() + 3, game_name.size());
                return game_name.size();
            }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* join_game = dynamic_cast<JoinGameRequest*>(msg.get());
    ASSERT_NE(join_game, nullptr);
    EXPECT_EQ(join_game->get_game_name(), game_name);
}

// JoinGameResponse Test Cases

TEST(ProtocolTest, SendsJoinGameResponseCorrectly) {
    MockSocket mock_socket;

    const bool success = true;
    const JoinGameResponse req(success);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::JoinGameResponse);
    const uint16_t len = htons(1);
    memcpy(&serialized[1], &len, sizeof(len));
    serialized[3] = static_cast<uint8_t>(success);

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesJoinGameResponseCorrectly) {
    MockSocket mock_socket;

    const bool success = true;
    const JoinGameResponse req(success);
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::JoinGameResponse);
    const uint16_t len = htons(1);
    memcpy(&serialized[1], &len, sizeof(len));
    serialized[3] = static_cast<uint8_t>(success);

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    EXPECT_CALL(mock_socket, recvall(_, 1)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data() + 3, 1);
        return 1;
    }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* join_game_resp = dynamic_cast<JoinGameResponse*>(msg.get());
    ASSERT_NE(join_game_resp, nullptr);
    EXPECT_EQ(join_game_resp->get_success(), success);
}

// GameListRequest Test Cases

TEST(ProtocolTest, SendsGameListRequestCorrectly) {
    MockSocket mock_socket;

    const GameListRequest req;
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::GameListRequest);
    const uint16_t len = htons(0);
    memcpy(&serialized[1], &len, sizeof(len));

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesGameListRequestCorrectly) {
    MockSocket mock_socket;

    const GameListRequest req;
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::GameListRequest);
    const uint16_t len = htons(0);
    memcpy(&serialized[1], &len, sizeof(len));

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* game_list_req = dynamic_cast<GameListRequest*>(msg.get());
    ASSERT_NE(game_list_req, nullptr);
}

// GameListResponse Test Cases

TEST(ProtocolTest, SendsGameListResponseCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const std::string map_name = "a map";
    uint8_t current_players = 1;
    uint8_t max_players = 2;

    const GameInfo game_one{game_name, map_name, current_players, max_players};
    const std::list<GameInfo> games = {game_one};
    const GameListResponse req(games);

    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::GameListResponse);
    size_t offset = 3;
    for (const auto& game: games) {
        uint16_t name_length = htons(static_cast<uint16_t>(game.name.size()));
        std::memcpy(serialized.data() + offset, &name_length, sizeof(name_length));
        offset += sizeof(name_length);
        std::memcpy(serialized.data() + offset, game.name.data(), game.name.size());
        offset += game.name.size();

        uint16_t map_name_length = htons(static_cast<uint16_t>(game.map_name.size()));
        std::memcpy(serialized.data() + offset, &map_name_length, sizeof(map_name_length));
        offset += sizeof(map_name_length);
        std::memcpy(serialized.data() + offset, game.map_name.data(), game.map_name.size());
        offset += game.map_name.size();

        serialized[offset++] = game.current_players;
        serialized[offset++] = game.max_players;
    }
    const uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
    std::memcpy(serialized.data() + 1, &payload_length, sizeof(payload_length));

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesGameListResponseCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const std::string map_name = "a map";
    const uint8_t current_players = 1;
    const uint8_t max_players = 2;

    GameInfo game_one{game_name, map_name, current_players, max_players};
    std::list<GameInfo> games = {game_one};
    GameListResponse req(games);

    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::GameListResponse);
    size_t offset = 3;
    for (const auto& game: games) {
        uint16_t name_length = htons(static_cast<uint16_t>(game.name.size()));
        std::memcpy(serialized.data() + offset, &name_length, sizeof(name_length));
        offset += sizeof(name_length);
        std::memcpy(serialized.data() + offset, game.name.data(), game.name.size());
        offset += game.name.size();

        uint16_t map_name_length = htons(static_cast<uint16_t>(game.map_name.size()));
        std::memcpy(serialized.data() + offset, &map_name_length, sizeof(map_name_length));
        offset += sizeof(map_name_length);
        std::memcpy(serialized.data() + offset, game.map_name.data(), game.map_name.size());
        offset += game.map_name.size();

        serialized[offset++] = game.current_players;
        serialized[offset++] = game.max_players;
    }
    uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
    std::memcpy(serialized.data() + 1, &payload_length, sizeof(payload_length));

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    EXPECT_CALL(mock_socket, recvall(_, serialized.size() - 3))
            .WillOnce(Invoke([&](void* dst, unsigned int) {
                memcpy(dst, serialized.data() + 3, serialized.size() - 3);
                return serialized.size() - 3;
            }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* game_list_resp = dynamic_cast<GameListResponse*>(msg.get());
    ASSERT_NE(game_list_resp, nullptr);
    const auto& games_deserialized = game_list_resp->get_games();
    ASSERT_EQ(games_deserialized.size(), 1);
    auto it = games_deserialized.begin();
    EXPECT_EQ(it->name, game_name);
    EXPECT_EQ(it->map_name, map_name);
    EXPECT_EQ(it->current_players, current_players);
    EXPECT_EQ(it->max_players, max_players);
}

// PlayerAction Test Cases
TEST(ProtocolTest, SendsPlayerActionCorrectly) {
    MockSocket mock_socket;

    const Action action = Action::MoveDown;
    int pos_mouse_x = 0;
    int pos_mouse_y = 0;
    const PlayerAction req(action, pos_mouse_x, pos_mouse_y);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::PlayerAction);
    const uint16_t len = htons(5);
    memcpy(&serialized[1], &len, sizeof(len));
    serialized[3] = static_cast<uint8_t>(action);
    uint16_t pos_mouse_x_be = htons(static_cast<uint16_t>(pos_mouse_x));
    uint16_t pos_mouse_y_be = htons(static_cast<uint16_t>(pos_mouse_y));
    memcpy(&serialized[4], &pos_mouse_x_be, sizeof(pos_mouse_x_be));
    memcpy(&serialized[6], &pos_mouse_y_be, sizeof(pos_mouse_y_be));

    EXPECT_CALL(mock_socket, sendall(_, serialized_size))
            .WillOnce(Invoke([&](const void* data, unsigned int) {
                EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
                return serialized_size;
            }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}

TEST(ProtocolTest, ReceivesPlayerActionCorrectly) {
    MockSocket mock_socket;

    const Action action = Action::MoveDown;
    int pos_mouse_x = 110;
    int pos_mouse_y = 110;
    const PlayerAction req(action, pos_mouse_x, pos_mouse_y);
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::PlayerAction);
    const uint16_t len = htons(5);
    memcpy(&serialized[1], &len, sizeof(len));
    serialized[3] = static_cast<uint8_t>(action);
    uint16_t pos_mouse_x_be = htons(static_cast<uint16_t>(pos_mouse_x));
    uint16_t pos_mouse_y_be = htons(static_cast<uint16_t>(pos_mouse_y));
    memcpy(&serialized[4], &pos_mouse_x_be, sizeof(pos_mouse_x_be));
    memcpy(&serialized[6], &pos_mouse_y_be, sizeof(pos_mouse_y_be));

    EXPECT_CALL(mock_socket, recvall(_, 3)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data(), 3);
        return 3;
    }));

    EXPECT_CALL(mock_socket, recvall(_, 5)).WillOnce(Invoke([&](void* dst, unsigned int) {
        memcpy(dst, serialized.data() + 3, 5);
        return 5;
    }));

    Protocol protocol(mock_socket);
    const std::unique_ptr<Message> msg = protocol.recv_message();

    auto* player_action = dynamic_cast<PlayerAction*>(msg.get());
    ASSERT_NE(player_action, nullptr);
    EXPECT_EQ(player_action->get_action(), action);
    EXPECT_EQ(player_action->get_pos_mouse_x(), pos_mouse_x);
    EXPECT_EQ(player_action->get_pos_mouse_y(), pos_mouse_y);
}
