#ifndef PLAYERDTO_H
#define PLAYERDTO_H
#include <string>
#include <utility>

#include "dto/player_info.h"

#include "player_skin.h"

class PlayerDTO {
public:
    const std::string username;
    const int position_x;
    const int position_y;
    const int aim_x;
    const int aim_y;
    const PlayerSkin skin;
    const int health;
    const Status status;
    const int money;
    int kills;
    int deaths;
    Weapon primary_weapon;
    uint16_t primary_weapon_ammo;
    Weapon secondary_weapon;
    uint16_t secondary_weapon_ammo;
    Weapon knife;
    Weapon bomb;
    Action last_action;
    Weapon current_weapon;
    int current_weapon_ammo;
    PlayerDTO(std::string username, const int position_x, const int position_y, const int aim_x,
              const int aim_y, const PlayerSkin skin, const int health, const Status status,
              const int money, const int kills, const int deaths, const Weapon primary_weapon,
              const int primary_weapon_ammo, const Weapon secondary_weapon,
              const int secondary_weapon_ammo, const Weapon knife, const Weapon bomb,
              const Action last_action, Weapon current_weapon, int current_weapon_ammo):
            username(std::move(username)),
            position_x(position_x),
            position_y(position_y),
            aim_x(aim_x),
            aim_y(aim_y),
            skin(skin),
            health(health),
            status(status),
            money(money),
            kills(kills),
            deaths(deaths),
            primary_weapon(primary_weapon),
            primary_weapon_ammo(primary_weapon_ammo),
            secondary_weapon(secondary_weapon),
            secondary_weapon_ammo(secondary_weapon_ammo),
            knife(knife),
            bomb(bomb),
            last_action(last_action),
            current_weapon(current_weapon),
            current_weapon_ammo(current_weapon_ammo) {}
};
#endif  // PLAYERDTO_H
