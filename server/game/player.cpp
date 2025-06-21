#include "player.h"

#include <algorithm>
#include <utility>

#include "../skin_translator.h"

#include "game_manager.h"


std::pair<int, int> Player::get_location() { return std::make_pair(position_x, position_y); }


void Player::set_location(const Position position, std::vector<std::pair<int, int>>&& chunks_idxs) {
    is_defusing = false;
    is_planting = false;
    auto [new_x, new_y] = position.get_position();
    position_x = new_x;
    position_y = new_y;

    this->chunks_idxs = std::move(chunks_idxs);
}


PlayerDTO Player::get_player_info() {
    auto last_action = Action::Idle;
    if (is_shooting) {
        last_action = Action::Shoot;
    }
    Weapon bomb_info = Weapon::None;
    if (bomb != nullptr) {
        bomb_info = Weapon::Bomb;
    }
    Weapon current_weapon;
    int current_weapon_ammo;
    WeaponInfo primary_info = get_primary_weapon_info();
    WeaponInfo secondary_info = get_secondary_weapon_info();
    if (equipped_weapon == GunType::Primary) {
        current_weapon = primary_info.weapon_type;
        current_weapon_ammo = primary_info.ammo;
    } else if (equipped_weapon == GunType::Secondary) {
        current_weapon = secondary_info.weapon_type;
        current_weapon_ammo = secondary_info.ammo;
    } else if (equipped_weapon == GunType::Knife) {
        current_weapon = Weapon::Knife;
        current_weapon_ammo = 0;
    } else {
        current_weapon = Weapon::Bomb;
        current_weapon_ammo = 0;
    }
    if (current_team == Team::Terrorists) {
        return PlayerDTO{username,
                         position_x,
                         position_y,
                         aim_x,
                         aim_y,
                         terrorist_skin,
                         health,
                         status,
                         money,
                         kills,
                         deaths,
                         primary_info.weapon_type,
                         primary_info.ammo,
                         secondary_info.weapon_type,
                         secondary_info.ammo,
                         Weapon::Knife,
                         bomb_info,
                         last_action,
                         current_weapon,
                         current_weapon_ammo};
    }
    return PlayerDTO{username,
                     position_x,
                     position_y,
                     aim_x,
                     aim_y,
                     ct_skin,
                     health,
                     status,
                     money,
                     kills,
                     deaths,
                     primary_info.weapon_type,
                     primary_info.ammo,
                     secondary_info.weapon_type,
                     secondary_info.ammo,
                     Weapon::Knife,
                     bomb_info,
                     last_action,
                     current_weapon,
                     current_weapon_ammo};
}


Team Player::get_team() const { return current_team; }


bool Player::is_dead() const { return status == Status::Dead; }


void Player::receive_damage(const GameManager& manager, const int damage) {
    health = std::max(0, health - damage);
    if (health == 0) {
        status = Status::Dead;
        deaths += 1;
        if (bomb != nullptr) {
            manager.drop_bomb(*this, std::move(bomb));
        } else if (equipped_weapon == GunType::Primary) {
            manager.drop_weapon(*this, std::move(primary_weapon));
        } else if (equipped_weapon == GunType::Secondary) {
            manager.drop_weapon(*this, std::move(secondary_weapon));
        }
        primary_weapon = nullptr;
        secondary_weapon = std::make_unique<Glock>(game_config.glock_config);
        equipped_weapon = GunType::Secondary;
    }
}

void Player::add_kill() {
    kills += 1;
    money += money_per_kill;
}


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


void Player::buy_ammo(const GunType gun_type) {
    if (money >= ammo_price) {
        if (gun_type == GunType::Primary && primary_weapon != nullptr) {
            primary_weapon->add_magazine();
            money -= ammo_price;
        } else if (gun_type == GunType::Secondary && secondary_weapon != nullptr) {
            secondary_weapon->add_magazine();
            money -= ammo_price;
        }
    }
}


void Player::switch_weapon(GunType gun_type) {
    is_planting = false;
    is_defusing = false;

    switch (equipped_weapon) {
        case GunType::Primary: {
            primary_weapon->reset_shoots();
            break;
        }
        case GunType::Secondary: {
            secondary_weapon->reset_shoots();
            break;
        }
        case GunType::Knife: {
            knife->reset_shoots();
            break;
        }
        case GunType::Bomb: {
            bomb->reset_shoots();
            break;
        }
    }

    if (gun_type == GunType::Primary && primary_weapon == nullptr) {
        return;
    }
    if (gun_type == GunType::Bomb && bomb == nullptr) {
        return;
    }
    equipped_weapon = gun_type;
}


void Player::reload() {
    is_defusing = false;
    is_planting = false;
    try {
        if (equipped_weapon == GunType::Primary) {
            primary_weapon->reload_gun();
        } else if (equipped_weapon == GunType::Secondary) {
            secondary_weapon->reload_gun();
        }
    } catch (const NoAmmo&) {}
}


void Player::switch_team() {
    if (current_team == Team::Terrorists) {
        current_team = Team::CounterTerrorists;
    } else {
        current_team = Team::Terrorists;
    }
}


void Player::shoot(const Position& pos, float time) {
    is_defusing = false;
    is_planting = false;
    try {
        if (equipped_weapon == GunType::Primary) {
            primary_weapon->shoot_gun(pos, time);
        } else if (equipped_weapon == GunType::Secondary) {
            secondary_weapon->shoot_gun(pos, time);
        } else if (equipped_weapon == GunType::Knife) {
            knife->shoot_gun(pos, time);
        } else if (equipped_weapon == GunType::Bomb) {
            bomb->shoot_gun(pos, time);
        }
    } catch (const NoAmmo&) {}
    auto [x, y] = pos.get_position();
    aim_x = x;
    aim_y = y;
}


Gun& Player::get_equipped_gun() const {
    switch (equipped_weapon) {
        case GunType::Primary: {
            return *primary_weapon;
        }
        case GunType::Secondary: {
            return *secondary_weapon;
        }
        case GunType::Knife: {
            return *knife;
        }
        case GunType::Bomb: {
            return *bomb;
        }
        default: {
            return *knife;
        }
    }
}


void Player::update(GameManager& game_manager) {
    if (status == Status::Dead) {
        return;
    }
    is_shooting = false;
    Map& map = game_manager.get_map();
    const double time = game_manager.get_time();
    Position pos(this->position_x, this->position_y);
    if (Gun& gun_to_shoot = get_equipped_gun(); gun_to_shoot.has_to_shoot(time)) {
        if (gun_to_shoot.get_gun_type() == GunType::Bomb) {
            if (game_manager.can_plant_bomb(position_x, position_y)) {
                gun_to_shoot.fire_gun(map, *this, time, pos);
            }
        }
        const ShootInfo shoots = gun_to_shoot.fire_gun(map, *this, time, pos);
        int x = 0;
        int y = 0;

        for (auto& shoot: shoots.results) {
            if (!shoot.miss && shoot.type != ShootType::PlantingBomb) {
                game_manager.attack_player(shoot.player_hit.value(), *this, shoot.dmg);
            }
            x = shoot.position.first;
            y = shoot.position.second;
        }
        if (shoots.results.size() == 1) {
            aim_x = x;
            aim_y = y;
        }
        is_shooting = true;
    }
    if (is_planting && bomb->has_finished_planting(time)) {
        game_manager.plant_bomb(position_x, position_y);
        equipped_weapon = GunType::Secondary;
        is_planting = false;
        bomb = nullptr;
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


void Player::start_planting() { is_planting = true; }


void Player::equip_weapon(std::unique_ptr<Gun> gun) {
    is_defusing = false;
    is_planting = false;
    const GunType type = gun->get_gun_type();
    if (type == GunType::Primary) {
        primary_weapon = std::move(gun);
    } else if (type == GunType::Secondary) {
        secondary_weapon = std::move(gun);
    }
    equipped_weapon = type;
}


WeaponInfo Player::get_primary_weapon_info() {
    if (primary_weapon == nullptr) {
        return WeaponInfo{Weapon::None, 0};
    }
    return primary_weapon->get_weapon_name();
}


WeaponInfo Player::get_secondary_weapon_info() {
    if (secondary_weapon == nullptr) {
        return WeaponInfo{Weapon::None, 0};
    }
    return secondary_weapon->get_weapon_name();
}
