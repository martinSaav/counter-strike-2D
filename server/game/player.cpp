#include "player.h"

#include <algorithm>
#include <utility>

#include "../skin_translator.h"

#include "game_manager.h"


std::pair<int, int> Player::get_location() { return std::make_pair(position_x, position_y); }


void Player::set_location(const Position position, std::vector<std::pair<int, int>>&& chunks_idxs) {
    auto [new_x, new_y] = position.get_position();
    position_x = new_x;
    position_y = new_y;

    this->chunks_idxs = std::move(chunks_idxs);
}


PlayerDTO Player::get_player_info() const {
    auto last_action = Action::MoveUp;
    if (is_shooting) {
        last_action = Action::Shoot;
    }
    if (current_team == Team::Terrorists) {
        return PlayerDTO{username, position_x, position_y, aim_x, aim_y,  terrorist_skin,
                         health,   status,     money,      kills, deaths, last_action};
    }
    return PlayerDTO{username, position_x, position_y, aim_x, aim_y,  ct_skin,
                     health,   status,     money,      kills, deaths, last_action};
}


Team Player::get_team() const { return current_team; }


bool Player::is_dead() const { return status == Status::Dead; }


void Player::receive_damage(const int damage) {
    health = std::max(0, health - damage);
    if (health == 0) {
        status = Status::Dead;
        deaths++;
        primary_weapon = nullptr;
        secondary_weapon = std::make_unique<Glock>();
        equipped_weapon = GunType::Secondary;
    }
}

void Player::add_kill() { kills++; }


void Player::buy_weapon(std::unique_ptr<Gun> gun) {
    if (const int price = gun->get_gun_price(); money >= price) {
        money -= price;
        if (gun->get_gun_type() == GunType::Primary) {
            primary_weapon = std::move(gun);
        } else {
            secondary_weapon = std::move(gun);
        }
    }
}


void Player::switch_weapon() {
    switch (equipped_weapon) {
        case GunType::Primary: {
            equipped_weapon = GunType::Secondary;
            primary_weapon->reset_shoots();
            break;
        }
        case GunType::Secondary: {
            equipped_weapon = GunType::Knife;
            secondary_weapon->reset_shoots();
            break;
        }
        default: {
            if (primary_weapon != nullptr) {
                equipped_weapon = GunType::Primary;
            } else {
                equipped_weapon = GunType::Secondary;
            }
            break;
        }
    }
}


void Player::reload() {
    try {
        if (equipped_weapon == GunType::Primary) {
            primary_weapon->reload_gun();
        } else if (equipped_weapon == GunType::Secondary) {
            secondary_weapon->reload_gun();
        }
    } catch (const NoAmmo&) {
        switch_weapon();
    }
}


void Player::switch_team() {
    if (current_team == Team::Terrorists) {
        current_team = Team::CounterTerrorists;
    } else {
        current_team = Team::Terrorists;
    }
}


void Player::shoot(const Position& pos) const {
    try {
        if (equipped_weapon == GunType::Primary) {
            primary_weapon->shoot_gun(pos);
        } else if (equipped_weapon == GunType::Secondary) {
            secondary_weapon->shoot_gun(pos);
        } else if (equipped_weapon == GunType::Knife) {
            knife->shoot_gun(pos);
        }
    } catch (const NoAmmo&) {}
}


std::unique_ptr<Gun>& Player::get_equipped_gun() {
    switch (equipped_weapon) {
        case GunType::Primary: {
            return primary_weapon;
        }
        case GunType::Secondary: {
            return secondary_weapon;
        }
        default: {
            return knife;
        }
    }
}


void Player::update(GameManager& game_manager) {
    is_shooting = false;
    Map& map = game_manager.get_map();
    const double time = game_manager.get_time();
    Position pos(this->position_x, this->position_y);
    if (const std::unique_ptr<Gun>& gun_to_shoot = get_equipped_gun();
        gun_to_shoot->has_to_shoot(time)) {
        const ShootResult shoot = gun_to_shoot->fire_gun(map, *this, time, pos);
        if (!shoot.miss) {
            game_manager.attack_player(shoot.player_hit.value(), *this, shoot.dmg);
        }
        auto [x, y] = shoot.position;
        aim_x = x;
        aim_y = y;
        is_shooting = true;
    }
}


std::pair<double, double> Player::get_center_coordinates() const {
    double pos_x = position_x + player_hitbox_width / 2;
    double pos_y = position_y + player_hitbox_height / 2;
    return std::make_pair(pos_x, pos_y);
}


void Player::set_skin(const PlayerSkin skin) {
    if (const Team team = SkinTranslator::get_skin_team(skin); team == Team::Terrorists) {
        terrorist_skin = skin;
    } else {
        ct_skin = skin;
    }
}
