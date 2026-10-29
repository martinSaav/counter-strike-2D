#ifndef JOIN_TEAM_REQUEST_H
#define JOIN_TEAM_REQUEST_H

#include "../message.h"
#include "../team.h"

class JoinTeamRequest: public MessageOf<MessageType::JoinTeamRequest> {
private:
    Team team;

public:
    explicit JoinTeamRequest(Team team): team(team) {}

    void write_payload(ByteWriter& writer) const override { writer.enum_u8(team); }

    static JoinTeamRequest read_payload(ByteReader& reader) {
        return JoinTeamRequest(reader.enum_u8<Team>());
    }

    Team get_team() const { return team; }
};
#endif
