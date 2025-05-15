#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <cstdint>

enum class MessageType : uint8_t {
    // Client -> Server
    LoginRequest        = 0x01,
    JoinGameRequest     = 0x02,
    CreateGameRequest   = 0x03,
    JoinTeamRequest     = 0x04,
    Ready               = 0x05,
    BuyWeaponRequest    = 0x06,
    BuyAmmoRequest      = 0x07,
    PlayerAction        = 0x08,
    DisconnectRequest   = 0x09,


    // Server -> Client
    LoginResponse      = 0x60,
    JoinGameResponse   = 0x61,
    CreateGameResponse = 0x62,
    JoinTeamResponse   = 0x63,
    GameStateUpdate = 0x64,

     // creo que este podria reemplazar a GameStarted, RoundStarted y RoundEnded o tambien el playerjoined y playerleft

     PlayerJoined     = 0x65,
     PlayerLeft       = 0x66,
     GameStarted      = 0x67,
     GameEnded        = 0x68,
     RoundStarted    = 0x69,
     RoundEnded      = 0x6A,
};

#endif
