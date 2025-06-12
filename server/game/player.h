#ifndef PLAYER_H
#define PLAYER_H
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../player_dto.h"
#include "dto/game_state_update.h"
#include "dto/player_info.h"
#include "gun/bomb_encapsulator.h"
#include "gun/glock.h"
#include "gun/gun.h"
#include "gun/gun_type.h"
#include "gun/knife.h"

#include "player_skin.h"
#include "position.h"
#include "weapon_info.h"
#define player_hitbox_height 3
#define player_hitbox_width 3
#define starting_money 500
#define max_health 100
#define money_per_kill 300

class GameManager;

class Bomb;

class Player {
    friend class GameManager;
    friend class Bomb;
    const std::string username;
    PlayerSkin terrorist_skin;
    PlayerSkin ct_skin;
    int health;
    int money;
    int current_angle;
    int position_x;
    int position_y;
    int aim_x;
    int aim_y;
    int kills;
    int deaths;
    bool is_shooting;
    bool is_planting;
    bool is_defusing;
    float defuse_time;
    Team current_team;
    Status status;
    std::unique_ptr<Gun> knife;
    std::unique_ptr<Gun> primary_weapon;
    std::unique_ptr<Gun> secondary_weapon;
    std::unique_ptr<BombEncapsulator> bomb;
    GunType equipped_weapon;
    std::vector<std::pair<int, int>>
            chunks_idxs;  // indices de los chunks en los que se encuentra el jugador
    void add_kill();
    void receive_damage(const GameManager& manager, int damage);
    void buy_weapon(std::unique_ptr<Gun> gun);
    void switch_weapon();
    std::unique_ptr<Gun>& get_equipped_gun();

public:
    Player(std::string username, const int position_x, const int position_y, const Team team):
            username(std::move(username)),
            health(max_health),
            money(starting_money),
            current_angle(0),
            position_x(position_x),
            position_y(position_y),
            aim_x(0),
            aim_y(0),
            kills(0),
            deaths(0),
            is_shooting(false),
            is_planting(false),
            is_defusing(false),
            defuse_time(0),
            current_team(team),
            status(Status::Alive) {
        terrorist_skin = PlayerSkin::T1;
        ct_skin = PlayerSkin::C1;
        chunks_idxs.emplace_back(0, 0);
        secondary_weapon = std::make_unique<Glock>();
        equipped_weapon = GunType::Secondary;
        knife = std::make_unique<Knife>();
    }
    std::pair<int, int> get_location();
    void set_location(Position position, std::vector<std::pair<int, int>>&& chunks_idxs);
    void set_aim_pos(const int x, const int y) {
        aim_x = x;
        aim_y = y;
    }
    void set_skin(PlayerSkin skin);
    [[nodiscard]] PlayerDTO get_player_info();
    [[nodiscard]] bool is_dead() const;
    void shoot(const Position& pos);
    std::vector<std::pair<int, int>>& get_chunk_idxs() { return chunks_idxs; }
    void update(GameManager& game_manager);
    [[nodiscard]] std::pair<double, double> get_center_coordinates() const;
    [[nodiscard]] Team get_team() const;
    void restore() {
        status = Status::Alive;
        bomb = nullptr;
        health = max_health;
    }
    void start_planting();
    void equip_bomb(std::unique_ptr<BombEncapsulator> bomb) { this->bomb = std::move(bomb); }
    void equip_weapon(std::unique_ptr<Gun> gun);
    [[nodiscard]] bool is_currently_defusing() const { return is_defusing; }
    void start_defusing() { is_defusing = true; }
    void reload();
    bool operator==(const Player& player) const = default;
    WeaponInfo get_primary_weapon_info();
    WeaponInfo get_secondary_weapon_info();
    [[nodiscard]] bool has_bomb() const { return bomb != nullptr; }
    void switch_team();
};
#endif  // PLAYER_H
