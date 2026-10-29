#ifndef MESSAGE_H
#define MESSAGE_H

#include "serialization/byte_reader.h"
#include "serialization/byte_writer.h"

#include "message_type.h"

// Un mensaje del protocolo. Solo sabe escribir sus campos (el payload); el encabezado con el tipo
// y el largo lo pone Protocol. Para leerlo, cada mensaje concreto expone
//     static X read_payload(ByteReader& reader);
// y se anota en message_registry.cpp.
class Message {
public:
    virtual ~Message() = default;

    virtual MessageType type() const = 0;
    virtual void write_payload(ByteWriter& writer) const = 0;
};

// Base de los mensajes concretos: fija el tipo en tiempo de compilacion (X::TYPE) y lo devuelve en
// type(), asi ningun mensaje tiene que guardarlo ni repetir el metodo.
template <MessageType Type>
class MessageOf: public Message {
public:
    static constexpr MessageType TYPE = Type;

    MessageType type() const override { return TYPE; }
};

#endif  // MESSAGE_H
