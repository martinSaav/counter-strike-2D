#ifndef JOIN_TEAM_REQUEST_H
#define JOIN_TEAM_REQUEST_H

#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../message.h"
#include "../message_type.h"
#include "game_state_update.h"

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

    size_t serialized_size() const override { return 4; }

    const Team& get_team() const { return team; }


    static JoinTeamRequest deserialize(const uint8_t* buffer, size_t size) {
        if (size < 3) {
            throw std::runtime_error("");
        }
        uint16_t length;
        memcpy(&length, buffer + 1, sizeof(length));
        size_t expected_size = 3 + static_cast<size_t>(length);
        if (size < expected_size) {
            throw std::runtime_error("");
        }
        Team team = static_cast<Team>(buffer[3]);
        return JoinTeamRequest(team);
    }

    MessageType type() const override { return this->message_type; }
};
#endif
