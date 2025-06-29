#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <cstdint>

#define HEADER_SIZE 3

enum class MessageType : uint8_t {
    // Client -> Server
    LoginRequest = 0x01,
    MapNamesRequest = 0x02,
    GameListRequest = 0x03,
    JoinGameRequest = 0x04,
    CreateGameRequest = 0x05,
    JoinTeamRequest = 0x06,
    SelectSkinRequest = 0x07,
    GameReadyRequest = 0x08,
    BuyWeaponRequest = 0x09,
    BuyAmmoRequest = 0x0A,
    PlayerAction = 0x0B,
    DisconnectRequest = 0x0C,


    // Server -> Client
    GameConfig = 0x20,
    LoginResponse = 0x60,
    MapNamesResponse = 0x61,
    GameListResponse = 0x62,
    JoinGameResponse = 0x63,
    CreateGameResponse = 0x64,
    JoinTeamResponse = 0x65,
    GameReadyResponse = 0x66,
    GameStateUpdate = 0x67,
};

#endif
