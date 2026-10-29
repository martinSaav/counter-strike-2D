#ifndef GAME_STATE_UPDATE_H
#define GAME_STATE_UPDATE_H

#include <cstdint>
#include <list>
#include <utility>

#include "../message.h"
#include "../round_stage.h"
#include "../team.h"

#include "dropped_weapon.h"
#include "player_info.h"


class GameStateUpdate: public MessageOf<MessageType::GameStateUpdate> {
private:
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
    RoundStage stage;
    float time_left;

public:
    explicit GameStateUpdate(bool game_started, bool game_ended, uint8_t round, float round_time,
                             bool round_started, bool round_ended, bool bomb_planted,
                             uint16_t bomb_x, uint16_t bomb_y, float bomb_timer, Team round_winner,
                             Team game_winner, std::list<PlayerInfo> players,
                             std::list<DroppedWeapon> dropped_weapons,
                             RoundStage stage = RoundStage::Buy, float time_left = 0):
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
            dropped_weapons(std::move(dropped_weapons)),
            stage(stage),
            time_left(time_left) {}


    void write_payload(ByteWriter& writer) const override {
        writer.boolean(game_started);
        writer.boolean(game_ended);
        writer.u8(round);
        writer.f32(round_time);
        writer.boolean(round_started);
        writer.boolean(round_ended);
        writer.boolean(bomb_planted);
        writer.u16(bomb_x);
        writer.u16(bomb_y);
        writer.f32(bomb_timer);
        writer.enum_u8(round_winner);
        writer.enum_u8(game_winner);
        writer.enum_u8(stage);
        writer.f32(time_left);
        writer.list(players, [](ByteWriter& w, const PlayerInfo& player) { player.write(w); });
        writer.list(dropped_weapons,
                    [](ByteWriter& w, const DroppedWeapon& weapon) { weapon.write(w); });
    }

    static GameStateUpdate read_payload(ByteReader& reader) {
        const bool game_started = reader.boolean();
        const bool game_ended = reader.boolean();
        const uint8_t round = reader.u8();
        const float round_time = reader.f32();
        const bool round_started = reader.boolean();
        const bool round_ended = reader.boolean();
        const bool bomb_planted = reader.boolean();
        const uint16_t bomb_x = reader.u16();
        const uint16_t bomb_y = reader.u16();
        const float bomb_timer = reader.f32();
        const auto round_winner = reader.enum_u8<Team>();
        const auto game_winner = reader.enum_u8<Team>();
        const auto stage = reader.enum_u8<RoundStage>();
        const float time_left = reader.f32();
        auto players = reader.list<std::list<PlayerInfo>>(PlayerInfo::read);
        auto dropped_weapons = reader.list<std::list<DroppedWeapon>>(DroppedWeapon::read);
        return GameStateUpdate(game_started, game_ended, round, round_time, round_started,
                               round_ended, bomb_planted, bomb_x, bomb_y, bomb_timer, round_winner,
                               game_winner, std::move(players), std::move(dropped_weapons), stage,
                               time_left);
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

    RoundStage get_stage() const { return stage; }

    float get_time_left() const { return time_left; }
};
#endif
