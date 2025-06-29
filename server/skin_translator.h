//
// Created by matias on 03/06/25.
//

#ifndef SKIN_TRANSLATOR_H
#define SKIN_TRANSLATOR_H
#include <map>
#include <stdexcept>
#include <string>

#include "dto/game_state_update.h"

#include "player_skin.h"

struct UnknownSkin: public std::runtime_error {
    UnknownSkin(): std::runtime_error("Unknown skin") {}
};


class SkinTranslator {
    static const std::map<std::string, PlayerSkin>& string_to_code_map();
    static const std::map<PlayerSkin, std::string>& code_to_string_map();
    static std::map<PlayerSkin, Team> skin_to_team_map();

public:
    static PlayerSkin string_to_code(const std::string& name);
    static std::string code_to_string(PlayerSkin skin);
    static Team get_skin_team(PlayerSkin skin);
};


#endif  // SKIN_TRANSLATOR_H
