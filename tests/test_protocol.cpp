#include "common/dto/create_game_request.h"
#include "common/dto/game_list_request.h"
#include "common/dto/game_list_response.h"
#include "common/dto/login_request.h"
#include "common/protocol.h"
#include "common/game.h"
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
    const CreateGameRequest req(game_name);
    const size_t serialized_size = req.serialized_size();
    std::vector<uint8_t> serialized(serialized_size);
    serialized[0] = static_cast<uint8_t>(MessageType::CreateGameRequest);
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

TEST(ProtocolTest, ReceivesCreateGameCorrectly) {
    MockSocket mock_socket;

    const std::string game_name = "a game";
    const CreateGameRequest req(game_name);
    std::vector<uint8_t> serialized(req.serialized_size());
    serialized[0] = static_cast<uint8_t>(MessageType::CreateGameRequest);
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

    auto* create_game = dynamic_cast<CreateGameRequest*>(msg.get());
    ASSERT_NE(create_game, nullptr);
    EXPECT_EQ(create_game->get_game_name(), game_name);
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

    const Game game_one{game_name, map_name, current_players, max_players};
    const std::list<Game> games = {game_one};
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

    Game game_one{game_name, map_name, current_players, max_players};
    std::list<Game> games = {game_one};
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