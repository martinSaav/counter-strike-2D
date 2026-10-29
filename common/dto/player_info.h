#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <cstdint>
#include <string>
#include <utility>

#include "../action.h"
#include "../serialization/byte_reader.h"
#include "../serialization/byte_writer.h"
#include "../weapon.h"

enum class Status : uint8_t {
    Alive = 0x01,
    Dead = 0x02,
};

class PlayerInfo {
private:
    std::string user_name;
    uint16_t pos_x;
    uint16_t pos_y;
    uint16_t health;
    Status status;
    uint16_t money;
    uint16_t kills;
    uint16_t deaths;
    Action action;
    uint16_t pos_shoot_x;
    uint16_t pos_shoot_y;
    std::string skin;
    Weapon primary_weapon;
    uint16_t primary_weapon_ammo;
    Weapon secondary_weapon;
    uint16_t secondary_weapon_ammo;
    Weapon knife;
    Weapon bomb;
    Weapon active_weapon;
    uint16_t active_weapon_ammo;
    // Avance (0-100) de la bomba que esta plantando o desactivando; 0 si no hace ninguna.
    uint8_t task_progress;

public:
    PlayerInfo(std::string user_name, uint16_t pos_x, uint16_t pos_y, uint16_t health,
               Status status, uint16_t money, uint16_t kills, uint16_t deaths, Action action,
               uint16_t pos_shoot_x, uint16_t pos_shoot_y, std::string skin, Weapon primary_weapon,
               uint16_t primary_weapon_ammo, Weapon secondary_weapon,
               uint16_t secondary_weapon_ammo, Weapon knife, Weapon bomb, Weapon active_weapon,
               uint16_t active_weapon_ammo, uint8_t task_progress = 0):
            user_name(std::move(user_name)),
            pos_x(pos_x),
            pos_y(pos_y),
            health(health),
            status(status),
            money(money),
            kills(kills),
            deaths(deaths),
            action(action),
            pos_shoot_x(pos_shoot_x),
            pos_shoot_y(pos_shoot_y),
            skin(std::move(skin)),
            primary_weapon(primary_weapon),
            primary_weapon_ammo(primary_weapon_ammo),
            secondary_weapon(secondary_weapon),
            secondary_weapon_ammo(secondary_weapon_ammo),
            knife(knife),
            bomb(bomb),
            active_weapon(active_weapon),
            active_weapon_ammo(active_weapon_ammo),
            task_progress(task_progress) {}

    void write(ByteWriter& writer) const {
        writer.string(user_name);
        writer.enum_u8(status);
        writer.u16(pos_x);
        writer.u16(pos_y);
        writer.u16(health);
        writer.u16(money);
        writer.u16(kills);
        writer.u16(deaths);
        writer.enum_u8(action);
        writer.u16(pos_shoot_x);
        writer.u16(pos_shoot_y);
        writer.string(skin);
        writer.enum_u8(primary_weapon);
        writer.u16(primary_weapon_ammo);
        writer.enum_u8(secondary_weapon);
        writer.u16(secondary_weapon_ammo);
        writer.enum_u8(knife);
        writer.enum_u8(bomb);
        writer.enum_u8(active_weapon);
        writer.u16(active_weapon_ammo);
        writer.u8(task_progress);
    }

    static PlayerInfo read(ByteReader& reader) {
        std::string user_name = reader.string();
        const auto status = reader.enum_u8<Status>();
        const uint16_t pos_x = reader.u16();
        const uint16_t pos_y = reader.u16();
        const uint16_t health = reader.u16();
        const uint16_t money = reader.u16();
        const uint16_t kills = reader.u16();
        const uint16_t deaths = reader.u16();
        const auto action = reader.enum_u8<Action>();
        const uint16_t pos_shoot_x = reader.u16();
        const uint16_t pos_shoot_y = reader.u16();
        std::string skin = reader.string();
        const auto primary_weapon = reader.enum_u8<Weapon>();
        const uint16_t primary_weapon_ammo = reader.u16();
        const auto secondary_weapon = reader.enum_u8<Weapon>();
        const uint16_t secondary_weapon_ammo = reader.u16();
        const auto knife = reader.enum_u8<Weapon>();
        const auto bomb = reader.enum_u8<Weapon>();
        const auto active_weapon = reader.enum_u8<Weapon>();
        const uint16_t active_weapon_ammo = reader.u16();
        const uint8_t task_progress = reader.u8();
        return PlayerInfo(std::move(user_name), pos_x, pos_y, health, status, money, kills, deaths,
                          action, pos_shoot_x, pos_shoot_y, std::move(skin), primary_weapon,
                          primary_weapon_ammo, secondary_weapon, secondary_weapon_ammo, knife, bomb,
                          active_weapon, active_weapon_ammo, task_progress);
    }

    const std::string& get_user_name() const { return user_name; }

    uint16_t get_pos_x() const { return pos_x; }

    uint16_t get_pos_y() const { return pos_y; }

    uint16_t get_health() const { return health; }

    Status get_status() const { return status; }

    uint16_t get_money() const { return money; }

    uint16_t get_kills() const { return kills; }

    uint16_t get_deaths() const { return deaths; }

    Action get_action() const { return action; }

    uint16_t get_pos_shoot_x() const { return pos_shoot_x; }

    uint16_t get_pos_shoot_y() const { return pos_shoot_y; }

    const std::string& get_skin() const { return skin; }

    Weapon get_primary_weapon() const { return primary_weapon; }

    uint16_t get_primary_weapon_ammo() const { return primary_weapon_ammo; }

    Weapon get_secondary_weapon() const { return secondary_weapon; }

    uint16_t get_secondary_weapon_ammo() const { return secondary_weapon_ammo; }

    Weapon get_knife() const { return knife; }

    Weapon get_bomb() const { return bomb; }

    Weapon get_active_weapon() const { return active_weapon; }

    uint16_t get_active_weapon_ammo() const { return active_weapon_ammo; }

    uint8_t get_task_progress() const { return task_progress; }
};

#endif
