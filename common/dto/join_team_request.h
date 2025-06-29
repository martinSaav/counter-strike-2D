#ifndef JOIN_TEAM_REQUEST_H
#define JOIN_TEAM_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"
#include "../team.h"

class JoinTeamRequest: public Message {
private:
    MessageType message_type = MessageType::JoinTeamRequest;
    Team team;

public:
    explicit JoinTeamRequest(Team team): team(team) {}

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        uint16_t length = htons(1);
        memcpy(buffer + 1, &length, sizeof(length));
        buffer[3] = static_cast<uint8_t>(team);
    }

    static JoinTeamRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        Team team = static_cast<Team>(buffer[3]);
        return JoinTeamRequest(team);
    }

    MessageType type() const override { return this->message_type; }

    size_t serialized_size() const override { return HEADER_SIZE + sizeof(team); }

    const Team& get_team() const { return team; }
};
#endif
