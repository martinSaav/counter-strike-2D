//
// Created by matias on 13/06/25.
//

#include "message_parser.h"

#include "commands/buy_weapon_command.h"
#include "commands/change_skin_command.h"
#include "commands/defuse_bomb_command.h"
#include "commands/equip_weapon_command.h"
#include "commands/game_ready_command.h"
#include "commands/leave_match_command.h"
#include "commands/movement_command.h"
#include "commands/reload_weapon_command.h"
#include "commands/shoot_command.h"
#include "commands/switch_weapon_command.h"
#include "dto/buy_weapon_request.h"
#include "dto/player_action.h"
#include "dto/select_skin_request.h"

#include "skin_translator.h"


MessageParser::MessageParser(const PlayerCredentials& credentials): credentials(credentials) {}


std::shared_ptr<PlayerCommand> MessageParser::parse_game_ready() {
    return std::make_shared<GameReadyCommand>(credentials);
}


std::shared_ptr<PlayerCommand> MessageParser::parse_buy_weapon(
        const std::unique_ptr<Message>& message) {
    auto buy_weapon_msg = dynamic_cast<BuyWeaponRequest*>(message.get());
    return std::make_shared<BuyWeaponCommand>(credentials, buy_weapon_msg->get_weapon());
}


std::shared_ptr<PlayerCommand> MessageParser::parse_select_skin(
        const std::unique_ptr<Message>& message) {
    const auto select_skin_msg = dynamic_cast<SelectSkinRequest*>(message.get());
    return std::make_shared<ChangeSkinCommand>(
            credentials, SkinTranslator::string_to_code(select_skin_msg->get_skin()));
}


std::shared_ptr<PlayerCommand> MessageParser::parse_disconnect() {
    return std::make_shared<LeaveMatchCommand>(credentials);
}


std::shared_ptr<PlayerCommand> MessageParser::parse_action(
        const std::unique_ptr<Message>& message) {
    const auto action_msg = dynamic_cast<PlayerAction*>(message.get());
    auto position = Position(action_msg->get_pos_mouse_x(), action_msg->get_pos_mouse_y());
    switch (action_msg->get_action()) {
        case Action::DefuseBomb: {
            return std::make_shared<DefuseBombCommand>(credentials);
        }
        case Action::EquipWeapon: {
            return std::make_shared<EquipWeaponCommand>(credentials);
        }
        case Action::Shoot: {
            return std::make_shared<ShootCommand>(credentials, position);
        }
        case Action::MoveDown: {
            return std::make_shared<MovementCommand>(credentials, CommandType::MoveDown, position);
        }
        case Action::MoveUp: {
            return std::make_shared<MovementCommand>(credentials, CommandType::MoveUp, position);
        }
        case Action::MoveLeft: {
            return std::make_shared<MovementCommand>(credentials, CommandType::MoveLeft, position);
        }
        case Action::MoveRight: {
            return std::make_shared<MovementCommand>(credentials, CommandType::MoveRight, position);
        }
        case Action::Reload: {
            return std::make_shared<ReloadWeaponCommand>(credentials);
        }
        case Action::SetKnife: {
            return std::make_shared<SwitchWeaponCommand>(credentials, GunType::Knife);
        }
        case Action::SetSecondaryWeapon: {
            return std::make_shared<SwitchWeaponCommand>(credentials, GunType::Secondary);
        }
        case Action::SetPrimaryWeapon: {
            return std::make_shared<SwitchWeaponCommand>(credentials, GunType::Primary);
        }
        default: {
            throw InvalidActionType();
        }
    }
}


std::shared_ptr<PlayerCommand> MessageParser::parse_message(
        const std::unique_ptr<Message>& message) {
    switch (message->type()) {
        case MessageType::GameReadyRequest: {
            return parse_game_ready();
        }
        case MessageType::BuyWeaponRequest: {
            return parse_buy_weapon(message);
        }
        case MessageType::SelectSkinRequest: {
            return parse_select_skin(message);
        }
        case MessageType::DisconnectRequest: {
            return parse_disconnect();
        }
        case MessageType::PlayerAction: {
            return parse_action(message);
        }
        default: {
            throw InvalidMessageType();
        }
    }
}
