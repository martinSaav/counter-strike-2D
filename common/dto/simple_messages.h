#ifndef SIMPLE_MESSAGES_H
#define SIMPLE_MESSAGES_H

#include "../message.h"
#include "../message_type.h"

// Pedido sin datos: alcanza con el tipo.
template <MessageType Type>
class EmptyMessage: public MessageOf<Type> {
public:
    void write_payload(ByteWriter&) const override {}

    static EmptyMessage read_payload(ByteReader&) { return EmptyMessage(); }
};

// Respuesta que solo dice si el pedido salio bien.
template <MessageType Type>
class SuccessResponse: public MessageOf<Type> {
private:
    bool success;

public:
    explicit SuccessResponse(bool success): success(success) {}

    void write_payload(ByteWriter& writer) const override { writer.boolean(success); }

    static SuccessResponse read_payload(ByteReader& reader) {
        return SuccessResponse(reader.boolean());
    }

    bool get_success() const { return success; }
};

using MapNamesRequest = EmptyMessage<MessageType::MapNamesRequest>;
using GameListRequest = EmptyMessage<MessageType::GameListRequest>;
using GameReadyRequest = EmptyMessage<MessageType::GameReadyRequest>;
using DisconnectRequest = EmptyMessage<MessageType::DisconnectRequest>;

using LoginResponse = SuccessResponse<MessageType::LoginResponse>;
using CreateGameResponse = SuccessResponse<MessageType::CreateGameResponse>;
using JoinGameResponse = SuccessResponse<MessageType::JoinGameResponse>;
using JoinTeamResponse = SuccessResponse<MessageType::JoinTeamResponse>;
using GameReadyResponse = SuccessResponse<MessageType::GameReadyResponse>;

#endif  // SIMPLE_MESSAGES_H
