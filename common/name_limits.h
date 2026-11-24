#ifndef NAME_LIMITS_H
#define NAME_LIMITS_H

#include <cstddef>

// Largo maximo (en bytes) de los nombres de partida y de usuario. Viajan en cada GameListResponse
// y GameStateUpdate, cuyo payload tiene un largo u16: sin un limite, un solo nombre enorme haria
// fallar el envio para todos los clientes.
constexpr std::size_t MAX_NAME_LENGTH = 32;

#endif  // NAME_LIMITS_H
