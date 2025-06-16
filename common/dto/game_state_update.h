#ifndef GAME_STATE_UPDATE_H
#define GAME_STATE_UPDATE_H

#include <cstdint>
#include <cstring>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../action.h"
#include "../message.h"
#include "../message_type.h"
#include "../team.h"
#include "../weapon.h"

#include "dropped_weapon.h"
#include "player_info.h"


class GameStateUpdate: public Message {
private:
    MessageType message_type = MessageType::GameStateUpdate;
    bool game_started;
    bool game_ended;
    uint8_t round;
    float round_time;
    bool round_started;
    bool round_ended;
    bool bomb_planted;
    uint16_t bomb_x;
    uint16_t bomb_y;
    float bomb_timer;
    Team round_winner;
    Team game_winner;
    std::list<PlayerInfo> players;
    std::list<DroppedWeapon> dropped_weapons;

public:
    explicit GameStateUpdate(bool game_started, bool game_ended, uint8_t round, float round_time,
                             bool round_started, bool round_ended, bool bomb_planted,
                             uint16_t bomb_x, uint16_t bomb_y, float bomb_timer, Team round_winner,
                             Team game_winner, std::list<PlayerInfo> players,
                             std::list<DroppedWeapon> dropped_weapons):
            game_started(game_started),
            game_ended(game_ended),
            round(round),
            round_time(round_time),
            round_started(round_started),
            round_ended(round_ended),
            bomb_planted(bomb_planted),
            bomb_x(bomb_x),
            bomb_y(bomb_y),
            bomb_timer(bomb_timer),
            round_winner(round_winner),
            game_winner(game_winner),
            players(std::move(players)),
            dropped_weapons(std::move(dropped_weapons)) {}


    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        size_t offset = HEADER_SIZE;

        buffer[offset++] = static_cast<uint8_t>(game_started);
        buffer[offset++] = static_cast<uint8_t>(game_ended);
        buffer[offset++] = round;

        std::memcpy(buffer + offset, &round_time, sizeof(round_time));
        offset += sizeof(round_time);

        buffer[offset++] = static_cast<uint8_t>(round_started);
        buffer[offset++] = static_cast<uint8_t>(round_ended);
        buffer[offset++] = static_cast<uint8_t>(bomb_planted);

        uint16_t bomb_x_net = htons(bomb_x);
        uint16_t bomb_y_net = htons(bomb_y);
        std::memcpy(buffer + offset, &bomb_x_net, sizeof(bomb_x_net));
        offset += sizeof(bomb_x_net);
        std::memcpy(buffer + offset, &bomb_y_net, sizeof(bomb_y_net));
        offset += sizeof(bomb_y_net);

        std::memcpy(buffer + offset, &bomb_timer, sizeof(bomb_timer));
        offset += sizeof(bomb_timer);

        buffer[offset++] = static_cast<uint8_t>(round_winner);
        buffer[offset++] = static_cast<uint8_t>(game_winner);

        uint16_t num_players = htons(static_cast<uint16_t>(players.size()));
        std::memcpy(buffer + offset, &num_players, sizeof(num_players));
        offset += sizeof(num_players);

        for (const auto& player: players) {
            player.serialize(buffer + offset);
            offset += player.serialized_size();
        }

        uint16_t num_weapons = htons(static_cast<uint16_t>(dropped_weapons.size()));
        std::memcpy(buffer + offset, &num_weapons, sizeof(num_weapons));
        offset += sizeof(num_weapons);

        for (const auto& weapon: dropped_weapons) {
            weapon.serialize(buffer + offset);
            offset += weapon.serialized_size();
        }

        uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
        std::memcpy(buffer + 1, &payload_length, sizeof(payload_length));
    }

    static GameStateUpdate deserialize(const uint8_t* buffer, size_t size) {
        size_t offset = HEADER_SIZE;

        bool game_started = buffer[offset++];
        bool game_ended = buffer[offset++];
        uint8_t round = buffer[offset++];

        float round_time;
        std::memcpy(&round_time, buffer + offset, sizeof(round_time));
        offset += sizeof(round_time);

        bool round_started = buffer[offset++];
        bool round_ended = buffer[offset++];
        bool bomb_planted = buffer[offset++];

        uint16_t bomb_x, bomb_y;
        std::memcpy(&bomb_x, buffer + offset, sizeof(bomb_x));
        bomb_x = ntohs(bomb_x);
        offset += sizeof(bomb_x);
        std::memcpy(&bomb_y, buffer + offset, sizeof(bomb_y));
        bomb_y = ntohs(bomb_y);
        offset += sizeof(bomb_y);

        float bomb_timer;
        std::memcpy(&bomb_timer, buffer + offset, sizeof(bomb_timer));
        offset += sizeof(bomb_timer);

        Team round_winner = static_cast<Team>(buffer[offset++]);
        Team game_winner = static_cast<Team>(buffer[offset++]);

        uint16_t num_players;
        std::memcpy(&num_players, buffer + offset, sizeof(num_players));
        num_players = ntohs(num_players);
        offset += sizeof(num_players);

        std::list<PlayerInfo> players;
        for (int i = 0; i < num_players; ++i) {
            PlayerInfo player = PlayerInfo::deserialize(buffer + offset, size - offset);
            offset += player.serialized_size();
            players.push_back(player);
        }

        uint16_t num_weapons;
        std::memcpy(&num_weapons, buffer + offset, sizeof(num_weapons));
        num_weapons = ntohs(num_weapons);
        offset += sizeof(num_weapons);

        std::list<DroppedWeapon> dropped_weapons;
        for (int i = 0; i < num_weapons; ++i) {
            DroppedWeapon weapon = DroppedWeapon::deserialize(buffer + offset, size - offset);
            offset += weapon.serialized_size();
            dropped_weapons.push_back(weapon);
        }

        return GameStateUpdate(game_started, game_ended, round, round_time, round_started,
                               round_ended, bomb_planted, bomb_x, bomb_y, bomb_timer, round_winner,
                               game_winner, std::move(players), std::move(dropped_weapons));
    }

    MessageType type() const override { return message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE;        // header
        size += 1 + 1 + 1;      // game_started, game_ended, round
        size += sizeof(float);  // round_time
        size += 1 + 1 + 1;      // round_started, round_ended, bomb_planted
        size += 2 + 2;          // bomb_x, bomb_y
        size += sizeof(float);  // bomb_timer
        size += 1 + 1;          // round_winner, game_winner
        size += 2;              // num_players
        size += std::accumulate(players.begin(), players.end(), 0,
                                [](const size_t sum, const PlayerInfo& player) {
                                    return sum + player.serialized_size();
                                });
        size += 2;              // num_weapons
        size += std::accumulate(dropped_weapons.begin(), dropped_weapons.end(), 0,
                                [](const size_t sum, const DroppedWeapon& weapon) {
                                    return sum + weapon.serialized_size();
                                });
        return size;
    }

    std::list<PlayerInfo> get_players() const { return players; }

    bool is_game_started() const { return game_started; }

    bool is_game_ended() const { return game_ended; }

    uint8_t get_round() const { return round; }

    float get_round_time() const { return round_time; }

    bool is_round_started() const { return round_started; }

    bool is_round_ended() const { return round_ended; }

    bool is_bomb_planted() const { return bomb_planted; }

    uint16_t get_bomb_x() const { return bomb_x; }

    uint16_t get_bomb_y() const { return bomb_y; }

    float get_bomb_timer() const { return bomb_timer; }

    Team get_round_winner() const { return round_winner; }

    Team get_game_winner() const { return game_winner; }

    std::list<DroppedWeapon> get_dropped_weapons() const { return dropped_weapons; }
};
#endif
