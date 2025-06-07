#ifndef PLAYER_H
#define PLAYER_H
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../player_dto.h"
#include "dto/game_state_update.h"
#include "dto/player_info.h"
#include "gun/glock.h"
#include "gun/gun.h"
#include "gun/gun_type.h"
#include "gun/knife.h"

#include "player_skin.h"
#include "position.h"
#define player_hitbox_height 32
#define player_hitbox_width 32
#define starting_money 500
#define max_health 100

class GameManager;

class Player {
    friend class GameManager;
    const std::string username;
    PlayerSkin terrorist_skin;
    PlayerSkin ct_skin;
    int health;
    int money;
    int current_angle;
    int position_x;
    int position_y;
    int kills;
    int deaths;
    Team current_team;
    Status status;
    std::unique_ptr<Gun> knife;
    std::unique_ptr<Gun> primary_weapon;
    std::unique_ptr<Gun> secondary_weapon;
    GunType equipped_weapon;
    std::vector<std::pair<int, int>>
            chunks_idxs;  // indices de los chunks en los que se encuentra el jugador
    void switch_team();
    [[nodiscard]] Team get_team() const;
    void add_kill();
    void receive_damage(int damage);
    void buy_weapon(std::unique_ptr<Gun> gun);
    void switch_weapon();
    void reload();

public:
    Player(std::string username, const int position_x, const int position_y, const Team team):
            username(std::move(username)),
            health(max_health),
            money(starting_money),
            current_angle(0),
            position_x(position_x),
            position_y(position_y),
            kills(0),
            deaths(0),
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
    void set_angle(const int angle) { current_angle = angle; }
    void set_skin(PlayerSkin skin);
    [[nodiscard]] PlayerDTO get_player_info() const;
    [[nodiscard]] bool is_dead() const;
    void shoot(const Position& pos) const;
    std::vector<std::pair<int, int>>& get_chunk_idxs() { return chunks_idxs; }
    void update(GameManager& game_manager);
    [[nodiscard]] std::pair<double, double> get_center_coordinates() const;

    bool operator==(const Player& player) const = default;
};
#endif  // PLAYER_H
