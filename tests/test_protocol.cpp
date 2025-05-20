#include "common/dto/login_request.h"
#include "common/dto/create_game_request.h"
#include "common/protocol.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "mock_socket.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

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

    EXPECT_CALL(mock_socket, sendall(_, serialized_size)).WillOnce(Invoke([&](const void* data, unsigned int) {
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

    EXPECT_CALL(mock_socket, sendall(_, serialized_size)).WillOnce(Invoke([&](const void* data, unsigned int) {
        EXPECT_EQ(memcmp(data, serialized.data(), serialized_size), 0);
        return serialized_size;
    }));

    Protocol protocol(mock_socket);
    protocol.send_message(req);
}
