#ifndef MESSAGE_REGISTRY_H
#define MESSAGE_REGISTRY_H

#include <memory>

#include "serialization/byte_reader.h"

#include "message.h"
#include "message_type.h"

// Arma el mensaje de tipo `type` leyendo su payload. Lanza ProtocolError si el tipo no existe o
// si el payload no corresponde a ese mensaje (le faltan bytes o le sobran).
std::unique_ptr<Message> read_message(MessageType type, ByteReader& payload);

#endif  // MESSAGE_REGISTRY_H
