#ifndef GAME_CONFIG_INFO_H
#define GAME_CONFIG_INFO_H

#include <cstdint>
#include <cstring>
#include <utility>

#include <arpa/inet.h>

#include "../message.h"
#include "../message_type.h"
#include "../weapon.h"

#include "gun_config_info.h"
#include "map_config_info.h"

class GameConfigInfo: public Message {
private:
    MessageType message_type = MessageType::GameConfig;
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
            knife_config(std::move(knife_config)),
            glock_config(std::move(glock_config)),
            ak_config(std::move(ak_config)),
            awp_config(std::move(awp_config)),
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

    void serialize(uint8_t* buffer) const override {
        buffer[0] = static_cast<uint8_t>(message_type);
        size_t offset = HEADER_SIZE;

        int32_t net_player_health = htonl(player_health);
        std::memcpy(buffer + offset, &net_player_health, sizeof(net_player_health));
        offset += sizeof(net_player_health);

        int32_t net_number_of_rounds = htonl(number_of_rounds);
        std::memcpy(buffer + offset, &net_number_of_rounds, sizeof(net_number_of_rounds));
        offset += sizeof(net_number_of_rounds);

        int32_t net_starting_money = htonl(starting_money);
        std::memcpy(buffer + offset, &net_starting_money, sizeof(net_starting_money));
        offset += sizeof(net_starting_money);

        int32_t net_ct_amount = htonl(ct_amount);
        std::memcpy(buffer + offset, &net_ct_amount, sizeof(net_ct_amount));
        offset += sizeof(net_ct_amount);

        int32_t net_tt_amount = htonl(tt_amount);
        std::memcpy(buffer + offset, &net_tt_amount, sizeof(net_tt_amount));
        offset += sizeof(net_tt_amount);

        int32_t net_ammo_price = htonl(ammo_price);
        std::memcpy(buffer + offset, &net_ammo_price, sizeof(net_ammo_price));
        offset += sizeof(net_ammo_price);

        knife_config.serialize(buffer + offset);
        offset += knife_config.serialized_size();

        glock_config.serialize(buffer + offset);
        offset += glock_config.serialized_size();

        ak_config.serialize(buffer + offset);
        offset += ak_config.serialized_size();

        awp_config.serialize(buffer + offset);
        offset += awp_config.serialized_size();

        m3_config.serialize(buffer + offset);
        offset += m3_config.serialized_size();

        int32_t net_defuse_time = htonl(defuse_time);
        std::memcpy(buffer + offset, &net_defuse_time, sizeof(net_defuse_time));
        offset += sizeof(net_defuse_time);

        int32_t net_time_to_plant = htonl(time_to_plant);
        std::memcpy(buffer + offset, &net_time_to_plant, sizeof(net_time_to_plant));
        offset += sizeof(net_time_to_plant);

        int32_t net_bomb_dmg = htonl(bomb_dmg);
        std::memcpy(buffer + offset, &net_bomb_dmg, sizeof(net_bomb_dmg));
        offset += sizeof(net_bomb_dmg);

        int32_t net_round_winner_money = htonl(round_winner_money);
        std::memcpy(buffer + offset, &net_round_winner_money, sizeof(net_round_winner_money));
        offset += sizeof(net_round_winner_money);

        int32_t net_round_loser_money = htonl(round_loser_money);
        std::memcpy(buffer + offset, &net_round_loser_money, sizeof(net_round_loser_money));
        offset += sizeof(net_round_loser_money);

        int32_t net_buy_time = htonl(buy_time);
        std::memcpy(buffer + offset, &net_buy_time, sizeof(net_buy_time));
        offset += sizeof(net_buy_time);

        int32_t net_bomb_time = htonl(bomb_time);
        std::memcpy(buffer + offset, &net_bomb_time, sizeof(net_bomb_time));
        offset += sizeof(net_bomb_time);

        int32_t net_after_round_time = htonl(after_round_time);
        std::memcpy(buffer + offset, &net_after_round_time, sizeof(net_after_round_time));
        offset += sizeof(net_after_round_time);

        int32_t net_money_per_kill = htonl(money_per_kill);
        std::memcpy(buffer + offset, &net_money_per_kill, sizeof(net_money_per_kill));
        offset += sizeof(net_money_per_kill);

        int32_t net_tiles_per_movement = htonl(tiles_per_movement);
        std::memcpy(buffer + offset, &net_tiles_per_movement, sizeof(net_tiles_per_movement));
        offset += sizeof(net_tiles_per_movement);

        int32_t net_game_rate = htonl(game_rate);
        std::memcpy(buffer + offset, &net_game_rate, sizeof(net_game_rate));
        offset += sizeof(net_game_rate);

        map_config.serialize(buffer + offset);
        offset += map_config.serialized_size();

        int32_t cone_angle_net = htonl(cone_angle);
        memcpy(buffer + offset, &cone_angle_net, sizeof(cone_angle_net));
        offset += sizeof(cone_angle_net);

        int32_t opacity_net = htonl(opacity);
        memcpy(buffer + offset, &opacity_net, sizeof(opacity_net));
        offset += sizeof(opacity_net);

        uint16_t payload_length = htons(static_cast<uint16_t>(offset - 3));
        std::memcpy(buffer + 1, &payload_length, sizeof(payload_length));
    }

    static GameConfigInfo deserialize(const uint8_t* buffer, size_t size) {
        size_t offset = HEADER_SIZE;

        int32_t player_health;
        std::memcpy(&player_health, buffer + offset, sizeof(player_health));
        player_health = ntohl(player_health);
        offset += sizeof(player_health);

        int32_t number_of_rounds;
        std::memcpy(&number_of_rounds, buffer + offset, sizeof(number_of_rounds));
        number_of_rounds = ntohl(number_of_rounds);
        offset += sizeof(number_of_rounds);

        int32_t starting_money;
        std::memcpy(&starting_money, buffer + offset, sizeof(starting_money));
        starting_money = ntohl(starting_money);
        offset += sizeof(starting_money);

        int32_t ct_amount;
        std::memcpy(&ct_amount, buffer + offset, sizeof(ct_amount));
        ct_amount = ntohl(ct_amount);
        offset += sizeof(ct_amount);

        int32_t tt_amount;
        std::memcpy(&tt_amount, buffer + offset, sizeof(tt_amount));
        tt_amount = ntohl(tt_amount);
        offset += sizeof(tt_amount);

        int32_t ammo_price;
        std::memcpy(&ammo_price, buffer + offset, sizeof(ammo_price));
        ammo_price = ntohl(ammo_price);
        offset += sizeof(ammo_price);

        GunConfigInfo knife_config = GunConfigInfo::deserialize(buffer + offset, size - offset);
        offset += knife_config.serialized_size();

        GunConfigInfo glock_config = GunConfigInfo::deserialize(buffer + offset, size - offset);
        offset += glock_config.serialized_size();

        GunConfigInfo ak_config = GunConfigInfo::deserialize(buffer + offset, size - offset);
        offset += ak_config.serialized_size();

        GunConfigInfo awp_config = GunConfigInfo::deserialize(buffer + offset, size - offset);
        offset += awp_config.serialized_size();

        GunConfigInfo m3_config = GunConfigInfo::deserialize(buffer + offset, size - offset);
        offset += m3_config.serialized_size();

        int32_t defuse_time;
        std::memcpy(&defuse_time, buffer + offset, sizeof(defuse_time));
        defuse_time = ntohl(defuse_time);
        offset += sizeof(defuse_time);

        int32_t time_to_plant;
        std::memcpy(&time_to_plant, buffer + offset, sizeof(time_to_plant));
        time_to_plant = ntohl(time_to_plant);
        offset += sizeof(time_to_plant);

        int32_t bomb_dmg;
        std::memcpy(&bomb_dmg, buffer + offset, sizeof(bomb_dmg));
        bomb_dmg = ntohl(bomb_dmg);
        offset += sizeof(bomb_dmg);

        int32_t round_winner_money;
        std::memcpy(&round_winner_money, buffer + offset, sizeof(round_winner_money));
        round_winner_money = ntohl(round_winner_money);
        offset += sizeof(round_winner_money);

        int32_t round_loser_money;
        std::memcpy(&round_loser_money, buffer + offset, sizeof(round_loser_money));
        round_loser_money = ntohl(round_loser_money);
        offset += sizeof(round_loser_money);

        int32_t buy_time;
        std::memcpy(&buy_time, buffer + offset, sizeof(buy_time));
        buy_time = ntohl(buy_time);
        offset += sizeof(buy_time);

        int32_t bomb_time;
        std::memcpy(&bomb_time, buffer + offset, sizeof(bomb_time));
        bomb_time = ntohl(bomb_time);
        offset += sizeof(bomb_time);

        int32_t after_round_time;
        std::memcpy(&after_round_time, buffer + offset, sizeof(after_round_time));
        after_round_time = ntohl(after_round_time);
        offset += sizeof(after_round_time);

        int32_t money_per_kill;
        std::memcpy(&money_per_kill, buffer + offset, sizeof(money_per_kill));
        money_per_kill = ntohl(money_per_kill);
        offset += sizeof(money_per_kill);

        int32_t tiles_per_movement;
        std::memcpy(&tiles_per_movement, buffer + offset, sizeof(tiles_per_movement));
        tiles_per_movement = ntohl(tiles_per_movement);
        offset += sizeof(tiles_per_movement);

        int32_t game_rate;
        std::memcpy(&game_rate, buffer + offset, sizeof(game_rate));
        game_rate = ntohl(game_rate);
        offset += sizeof(game_rate);

        MapConfigInfo map_config = MapConfigInfo::deserialize(buffer + offset, size - offset);
        offset += map_config.serialized_size();

        int32_t cone_angle;
        memcpy(&cone_angle, buffer + offset, sizeof(cone_angle));
        cone_angle = ntohl(cone_angle);
        offset += sizeof(cone_angle);

        int32_t opacity;
        memcpy(&opacity, buffer + offset, sizeof(opacity));
        opacity = ntohl(opacity);

        return GameConfigInfo(player_health, number_of_rounds, starting_money, ct_amount, tt_amount,
                              ammo_price, std::move(knife_config), std::move(glock_config),
                              std::move(ak_config), std::move(awp_config), std::move(m3_config),
                              defuse_time, time_to_plant, bomb_dmg, round_winner_money,
                              round_loser_money, buy_time, bomb_time, after_round_time,
                              money_per_kill, tiles_per_movement, game_rate, std::move(map_config),
                              cone_angle, opacity);
    }

    MessageType type() const override { return message_type; }

    size_t serialized_size() const override {
        size_t size = HEADER_SIZE;
        size += sizeof(player_health);
        size += sizeof(number_of_rounds);
        size += sizeof(starting_money);
        size += sizeof(ct_amount);
        size += sizeof(tt_amount);
        size += sizeof(ammo_price);
        size += knife_config.serialized_size();
        size += glock_config.serialized_size();
        size += ak_config.serialized_size();
        size += awp_config.serialized_size();
        size += m3_config.serialized_size();
        size += sizeof(defuse_time);
        size += sizeof(time_to_plant);
        size += sizeof(bomb_dmg);
        size += sizeof(round_winner_money);
        size += sizeof(round_loser_money);
        size += sizeof(buy_time);
        size += sizeof(bomb_time);
        size += sizeof(after_round_time);
        size += sizeof(money_per_kill);
        size += sizeof(tiles_per_movement);
        size += sizeof(game_rate);
        size += map_config.serialized_size();
        size += sizeof(cone_angle);
        size += sizeof(opacity);
        return size;
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
