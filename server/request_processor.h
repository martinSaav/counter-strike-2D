//
// Created by matias on 12/06/25.
//

#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H
#include "gun/gun_type.h"

#include "command_type.h"
#include "player_credentials.h"
#include "player_skin.h"
#include "position.h"
#include "weapon.h"

class Match;

class RequestProcessor {
public:
    static void process_movement_request(PlayerCredentials credentials, CommandType command,
                                         Position aim_pos, Match* match);
    static void process_shoot_request(PlayerCredentials credentials, Position aim_pos,
                                      Match* match);
    static void process_leave_match_request(PlayerCredentials credentials, Match* match);
    static void process_change_skin_request(PlayerCredentials credentials, PlayerSkin new_skin,
                                            Match* match);
    static void process_game_ready_request(Match* match);
    static void process_buy_weapon_request(PlayerCredentials credentials, Weapon weapon,
                                           Match* match);
    static void process_switch_weapon_request(PlayerCredentials credentials, GunType gun_type,
                                              Match* match);
    static void process_reload_request(PlayerCredentials credentials, Match* match);
    static void process_pick_weapon_request(PlayerCredentials credentials, Match* match);
    static void process_defuse_request(PlayerCredentials credentials, Match* match);
};


#endif  // REQUEST_PROCESSOR_H
