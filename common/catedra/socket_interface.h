#ifndef SOCKET_INTERFACE_H
#define SOCKET_INTERFACE_H

class SocketInterface {
public:
    virtual ~SocketInterface();
    virtual int sendsome(const void* data, unsigned int size) = 0;
    virtual int sendall(const void* data, unsigned int size) = 0;
    virtual int recvsome(void* data, unsigned int size) = 0;
    virtual int recvall(void* data, unsigned int size) = 0;
    virtual void shutdown(int how) = 0;
    virtual int close() = 0;
};

#endif
