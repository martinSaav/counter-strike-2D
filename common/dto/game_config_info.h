#ifndef GAME_CONFIG_INFO_H
#define GAME_CONFIG_INFO_H

#include <cstdint>
#include <utility>

#include "../message.h"

#include "gun_config_info.h"
#include "map_config_info.h"

class GameConfigInfo: public MessageOf<MessageType::GameConfig> {
private:
    int32_t player_health;
    int32_t number_of_rounds;
    int32_t starting_money;
    int32_t ct_amount;
    int32_t tt_amount;
    int32_t ammo_price;
    GunConfigInfo knife_config;
    GunConfigInfo glock_config;
    GunConfigInfo ak_config;
    GunConfigInfo awp_config;
    GunConfigInfo m3_config;
    int32_t defuse_time;
    int32_t time_to_plant;
    int32_t bomb_dmg;
    int32_t round_winner_money;
    int32_t round_loser_money;
    int32_t buy_time;
    int32_t bomb_time;
    int32_t after_round_time;
    int32_t money_per_kill;
    int32_t tiles_per_movement;
    int32_t game_rate;
    MapConfigInfo map_config;
    int32_t cone_angle;
    int32_t opacity;

public:
    GameConfigInfo(int32_t player_health, int32_t number_of_rounds, int32_t starting_money,
                   int32_t ct_amount, int32_t tt_amount, int32_t ammo_price,
                   GunConfigInfo knife_config, GunConfigInfo glock_config, GunConfigInfo ak_config,
                   GunConfigInfo awp_config, GunConfigInfo m3_config, int32_t defuse_time,
                   int32_t time_to_plant, int32_t bomb_dmg, int32_t round_winner_money,
                   int32_t round_loser_money, int32_t buy_time, int32_t bomb_time,
                   int32_t after_round_time, int32_t money_per_kill, int32_t tiles_per_movement,
                   int32_t game_rate, MapConfigInfo map_config, int32_t cone_angle,
                   int32_t opacity):
            player_health(player_health),
            number_of_rounds(number_of_rounds),
            starting_money(starting_money),
            ct_amount(ct_amount),
            tt_amount(tt_amount),
            ammo_price(ammo_price),
            knife_config(knife_config),
            glock_config(glock_config),
            ak_config(ak_config),
            awp_config(awp_config),
            m3_config(m3_config),
            defuse_time(defuse_time),
            time_to_plant(time_to_plant),
            bomb_dmg(bomb_dmg),
            round_winner_money(round_winner_money),
            round_loser_money(round_loser_money),
            buy_time(buy_time),
            bomb_time(bomb_time),
            after_round_time(after_round_time),
            money_per_kill(money_per_kill),
            tiles_per_movement(tiles_per_movement),
            game_rate(game_rate),
            map_config(std::move(map_config)),
            cone_angle(cone_angle),
            opacity(opacity) {}

    void write_payload(ByteWriter& writer) const override {
        writer.i32(player_health);
        writer.i32(number_of_rounds);
        writer.i32(starting_money);
        writer.i32(ct_amount);
        writer.i32(tt_amount);
        writer.i32(ammo_price);
        knife_config.write(writer);
        glock_config.write(writer);
        ak_config.write(writer);
        awp_config.write(writer);
        m3_config.write(writer);
        writer.i32(defuse_time);
        writer.i32(time_to_plant);
        writer.i32(bomb_dmg);
        writer.i32(round_winner_money);
        writer.i32(round_loser_money);
        writer.i32(buy_time);
        writer.i32(bomb_time);
        writer.i32(after_round_time);
        writer.i32(money_per_kill);
        writer.i32(tiles_per_movement);
        writer.i32(game_rate);
        map_config.write(writer);
        writer.i32(cone_angle);
        writer.i32(opacity);
    }

    static GameConfigInfo read_payload(ByteReader& reader) {
        // Los argumentos de una funcion se evaluan en cualquier orden, asi que cada campo se lee
        // en su propia linea para respetar el orden del mensaje
        const int32_t player_health = reader.i32();
        const int32_t number_of_rounds = reader.i32();
        const int32_t starting_money = reader.i32();
        const int32_t ct_amount = reader.i32();
        const int32_t tt_amount = reader.i32();
        const int32_t ammo_price = reader.i32();
        GunConfigInfo knife_config = GunConfigInfo::read(reader);
        GunConfigInfo glock_config = GunConfigInfo::read(reader);
        GunConfigInfo ak_config = GunConfigInfo::read(reader);
        GunConfigInfo awp_config = GunConfigInfo::read(reader);
        GunConfigInfo m3_config = GunConfigInfo::read(reader);
        const int32_t defuse_time = reader.i32();
        const int32_t time_to_plant = reader.i32();
        const int32_t bomb_dmg = reader.i32();
        const int32_t round_winner_money = reader.i32();
        const int32_t round_loser_money = reader.i32();
        const int32_t buy_time = reader.i32();
        const int32_t bomb_time = reader.i32();
        const int32_t after_round_time = reader.i32();
        const int32_t money_per_kill = reader.i32();
        const int32_t tiles_per_movement = reader.i32();
        const int32_t game_rate = reader.i32();
        MapConfigInfo map_config = MapConfigInfo::read(reader);
        const int32_t cone_angle = reader.i32();
        const int32_t opacity = reader.i32();
        return GameConfigInfo(player_health, number_of_rounds, starting_money, ct_amount, tt_amount,
                              ammo_price, knife_config, glock_config, ak_config, awp_config,
                              m3_config, defuse_time, time_to_plant, bomb_dmg, round_winner_money,
                              round_loser_money, buy_time, bomb_time, after_round_time,
                              money_per_kill, tiles_per_movement, game_rate, std::move(map_config),
                              cone_angle, opacity);
    }

    int32_t get_player_health() const { return player_health; }

    int32_t get_number_of_rounds() const { return number_of_rounds; }

    int32_t get_starting_money() const { return starting_money; }

    int32_t get_ct_amount() const { return ct_amount; }

    int32_t get_tt_amount() const { return tt_amount; }

    int32_t get_ammo_price() const { return ammo_price; }

    const GunConfigInfo& get_knife_config() const { return knife_config; }

    const GunConfigInfo& get_glock_config() const { return glock_config; }

    const GunConfigInfo& get_ak_config() const { return ak_config; }

    const GunConfigInfo& get_awp_config() const { return awp_config; }

    const GunConfigInfo& get_m3_config() const { return m3_config; }

    int32_t get_defuse_time() const { return defuse_time; }

    int32_t get_time_to_plant() const { return time_to_plant; }

    int32_t get_bomb_dmg() const { return bomb_dmg; }

    int32_t get_round_winner_money() const { return round_winner_money; }

    int32_t get_round_loser_money() const { return round_loser_money; }

    int32_t get_buy_time() const { return buy_time; }

    int32_t get_bomb_time() const { return bomb_time; }

    int32_t get_after_round_time() const { return after_round_time; }

    int32_t get_money_per_kill() const { return money_per_kill; }

    int32_t get_tiles_per_movement() const { return tiles_per_movement; }

    int32_t get_game_rate() const { return game_rate; }

    const MapConfigInfo& get_map_config() const { return map_config; }

    int32_t get_cone_angle() const { return cone_angle; }

    int32_t get_opacity() const { return opacity; }
};

#endif
