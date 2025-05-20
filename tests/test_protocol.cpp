#include "common/dto/login_request.h"
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
    std::vector<uint8_t> serialized(3 + username.size());
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
