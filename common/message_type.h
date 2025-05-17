#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <cstdint>

enum class MessageType : uint8_t {
    // Client -> Server
    LoginRequest = 0x01,
    MapNamesRequest = 0x02,
    GameListRequest = 0x03,
    JoinGameRequest = 0x04,
    CreateGameRequest = 0x05,
    JoinTeamRequest = 0x06,
    Ready = 0x07,
    BuyWeaponRequest = 0x08,
    BuyAmmoRequest = 0x09,
    PlayerAction = 0x0A,
    DisconnectRequest = 0x0B,


    // Server -> Client
    LoginResponse = 0x60,
    MapNamesResponse = 0x61,
    GameListResponse = 0x62,
    JoinGameResponse = 0x63,
    CreateGameResponse = 0x64,
    JoinTeamResponse = 0x65,
    GameStateUpdate = 0x66,  //{x y players[] round_number  }

    // creo que este podria reemplazar a GameStarted, RoundStarted y RoundEnded o tambien el
    // playerjoined y playerleft

    PlayerJoined = 0x67,
    PlayerLeft = 0x68,
    GameStarted = 0x69,
    GameEnded = 0x6A,
    RoundStarted = 0x6B,
    RoundEnded = 0x6C,
};

#endif
