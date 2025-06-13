//
// Created by matias on 12/06/25.
//

#include "request_processor.h"

#include "match.h"

void RequestProcessor::process_movement_request(const PlayerCredentials credentials,
                                                const CommandType command, const Position aim_pos,
                                                Match* match) {
    match->process_movement_request(credentials, command, aim_pos);
}


void RequestProcessor::process_shoot_request(const PlayerCredentials credentials,
                                             const Position aim_pos, Match* match) {
    match->process_shoot_request(credentials, aim_pos);
}


void RequestProcessor::process_leave_match_request(const PlayerCredentials credentials,
                                                   Match* match) {
    match->process_leave_match_request(credentials);
}


void RequestProcessor::process_change_skin_request(const PlayerCredentials credentials,
                                                   const PlayerSkin new_skin, Match* match) {
    match->process_change_skin_request(credentials, new_skin);
}


void RequestProcessor::process_game_ready_request(Match* match) {
    match->process_game_ready_request();
}


void RequestProcessor::process_buy_weapon_request(const PlayerCredentials credentials,
                                                  const Weapon weapon, Match* match) {
    match->process_buy_weapon_request(credentials, weapon);
}


void RequestProcessor::process_switch_weapon_request(const PlayerCredentials credentials,
                                                     const GunType gun_type, Match* match) {
    match->process_switch_weapon_request(credentials, gun_type);
}


void RequestProcessor::process_reload_request(const PlayerCredentials credentials, Match* match) {
    match->process_reload_request(credentials);
}


void RequestProcessor::process_pick_weapon_request(const PlayerCredentials credentials,
                                                   Match* match) {
    match->process_pick_weapon_request(credentials);
}


void RequestProcessor::process_defuse_request(const PlayerCredentials credentials, Match* match) {
    match->process_defuse_request(credentials);
}
