#ifndef PROTOCOL_ERROR_H
#define PROTOCOL_ERROR_H

#include <stdexcept>
#include <string>

// Un mensaje mal formado: truncado, con un largo que no entra o de un tipo desconocido.
struct ProtocolError: public std::runtime_error {
    explicit ProtocolError(const std::string& what): std::runtime_error(what) {}
};

#endif  // PROTOCOL_ERROR_H
