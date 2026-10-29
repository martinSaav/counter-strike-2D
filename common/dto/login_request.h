#ifndef LOGIN_REQUEST_H
#define LOGIN_REQUEST_H

#include <string>
#include <utility>

#include "../message.h"

class LoginRequest: public MessageOf<MessageType::LoginRequest> {
private:
    std::string username;

public:
    explicit LoginRequest(std::string username): username(std::move(username)) {}

    void write_payload(ByteWriter& writer) const override { writer.string(username); }

    static LoginRequest read_payload(ByteReader& reader) { return LoginRequest(reader.string()); }

    const std::string& get_username() const { return username; }
};
#endif
