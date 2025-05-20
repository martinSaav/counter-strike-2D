#ifndef MOCK_SOCKET_H
#define MOCK_SOCKET_H

#include <gmock/gmock.h>

#include "common/catedra/socket_interface.h"

class MockSocket: public SocketInterface {
public:
    MOCK_METHOD(int, sendsome, (const void* data, unsigned int size), (override));
    MOCK_METHOD(int, sendall, (const void* data, unsigned int size), (override));
    MOCK_METHOD(int, recvsome, (void* data, unsigned int size), (override));
    MOCK_METHOD(int, recvall, (void* data, unsigned int size), (override));
    MOCK_METHOD(void, shutdown, (int how), (override));
    MOCK_METHOD(int, close, (), (override));
};

#endif
