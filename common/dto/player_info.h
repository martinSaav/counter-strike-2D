#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

#include <netinet/in.h>

#include "../action.h"
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
    float money;
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

public:
    PlayerInfo(std::string user_name, uint16_t pos_x, uint16_t pos_y, uint16_t health,
               Status status, float money, uint16_t kills, uint16_t deaths, Action action,
               uint16_t pos_shoot_x, uint16_t pos_shoot_y, std::string skin, Weapon primary_weapon,
               uint16_t primary_weapon_ammo, Weapon secondary_weapon,
               uint16_t secondary_weapon_ammo, Weapon knife, Weapon bomb):
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
            bomb(bomb) {}

    void serialize(uint8_t* buffer) const {
        size_t offset = 0;
        uint16_t name_length = htons(static_cast<uint16_t>(user_name.size()));
        std::memcpy(buffer + offset, &name_length, sizeof(name_length));
        offset += sizeof(name_length);
        std::memcpy(buffer + offset, user_name.data(), user_name.size());
        offset += user_name.size();

        buffer[offset++] = static_cast<uint8_t>(status);
        std::memcpy(buffer + offset, &pos_x, sizeof(pos_x));
        offset += sizeof(pos_x);
        std::memcpy(buffer + offset, &pos_y, sizeof(pos_y));
        offset += sizeof(pos_y);
        std::memcpy(buffer + offset, &health, sizeof(health));
        offset += sizeof(health);
        std::memcpy(buffer + offset, &money, sizeof(money));
        offset += sizeof(money);
        std::memcpy(buffer + offset, &kills, sizeof(kills));
        offset += sizeof(kills);
        std::memcpy(buffer + offset, &deaths, sizeof(deaths));
        offset += sizeof(deaths);

        buffer[offset++] = static_cast<uint8_t>(action);
        std::memcpy(buffer + offset, &pos_shoot_x, sizeof(pos_shoot_x));
        offset += sizeof(pos_shoot_x);
        std::memcpy(buffer + offset, &pos_shoot_y, sizeof(pos_shoot_y));
        offset += sizeof(pos_shoot_y);
        uint16_t skin_length = htons(static_cast<uint16_t>(skin.size()));
        std::memcpy(buffer + offset, &skin_length, sizeof(skin_length));
        offset += sizeof(skin_length);
        std::memcpy(buffer + offset, skin.data(), skin.size());
        offset += skin.size();

        buffer[offset++] = static_cast<uint8_t>(primary_weapon);
        uint16_t primary_weapon_ammo_net = htons(primary_weapon_ammo);
        std::memcpy(buffer + offset, &primary_weapon_ammo_net, sizeof(primary_weapon_ammo_net));
        offset += sizeof(primary_weapon_ammo_net);
        buffer[offset++] = static_cast<uint8_t>(secondary_weapon);
        uint16_t secondary_weapon_ammo_net = htons(secondary_weapon_ammo);
        std::memcpy(buffer + offset, &secondary_weapon_ammo_net, sizeof(secondary_weapon_ammo_net));
        offset += sizeof(secondary_weapon_ammo_net);
        buffer[offset++] = static_cast<uint8_t>(knife);
        buffer[offset++] = static_cast<uint8_t>(bomb);
    }

    size_t serialized_size() const {
        return 2 + user_name.size() + 1 + 2 * 5 + sizeof(money) + 1 + 2 * 2 + 2 + skin.size() + 3;
    }

    const std::string& get_user_name() const { return user_name; }
    uint16_t get_pos_x() const { return pos_x; }
    uint16_t get_pos_y() const { return pos_y; }
    uint16_t get_health() const { return health; }
    Status get_status() const { return status; }
    float get_money() const { return money; }
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

    static PlayerInfo deserialize(const uint8_t* buffer, size_t size) {
        if (size < 2) {
            throw std::runtime_error("");
        }
        size_t offset = 0;
        uint16_t name_length;
        std::memcpy(&name_length, buffer + offset, sizeof(name_length));
        name_length = ntohs(name_length);
        offset += sizeof(name_length);

        std::string user_name(reinterpret_cast<const char*>(buffer + offset), name_length);
        offset += name_length;

        Status status = static_cast<Status>(buffer[offset++]);
        uint16_t pos_x, pos_y, health, kills, deaths;
        std::memcpy(&pos_x, buffer + offset, sizeof(pos_x));
        offset += sizeof(pos_x);
        std::memcpy(&pos_y, buffer + offset, sizeof(pos_y));
        offset += sizeof(pos_y);
        std::memcpy(&health, buffer + offset, sizeof(health));
        offset += sizeof(health);

        float money;
        std::memcpy(&money, buffer + offset, sizeof(money));
        offset += sizeof(money);

        std::memcpy(&kills, buffer + offset, sizeof(kills));
        offset += sizeof(kills);
        std::memcpy(&deaths, buffer + offset, sizeof(deaths));
        offset += sizeof(deaths);

        Action action = static_cast<Action>(buffer[offset++]);

        uint16_t pos_shoot_x, pos_shoot_y;
        std::memcpy(&pos_shoot_x, buffer + offset, sizeof(pos_shoot_x));
        offset += sizeof(pos_shoot_x);
        std::memcpy(&pos_shoot_y, buffer + offset, sizeof(pos_shoot_y));
        offset += sizeof(pos_shoot_y);

        uint16_t skin_length;
        std::memcpy(&skin_length, buffer + offset, sizeof(skin_length));
        skin_length = ntohs(skin_length);
        offset += sizeof(skin_length);

        std::string skin(reinterpret_cast<const char*>(buffer + offset), skin_length);
        offset += skin_length;

        Weapon primary_weapon = static_cast<Weapon>(buffer[offset++]);
        uint16_t primary_weapon_ammo;
        std::memcpy(&primary_weapon_ammo, buffer + offset, sizeof(primary_weapon_ammo));
        primary_weapon_ammo = ntohs(primary_weapon_ammo);
        offset += sizeof(primary_weapon_ammo);
        Weapon secondary_weapon = static_cast<Weapon>(buffer[offset++]);
        uint16_t secondary_weapon_ammo;
        std::memcpy(&secondary_weapon_ammo, buffer + offset, sizeof(secondary_weapon_ammo));
        secondary_weapon_ammo = ntohs(secondary_weapon_ammo);
        offset += sizeof(secondary_weapon_ammo);
        Weapon knife = static_cast<Weapon>(buffer[offset++]);
        Weapon bomb = static_cast<Weapon>(buffer[offset++]);

        return PlayerInfo(user_name, pos_x, pos_y, health, status, money, kills, deaths, action,
                          pos_shoot_x, pos_shoot_y, skin, primary_weapon, primary_weapon_ammo,
                          secondary_weapon, secondary_weapon_ammo, knife, bomb);
    }
};

#endif
