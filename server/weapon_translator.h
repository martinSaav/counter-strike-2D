//
// Created by matias on 23/06/25.
//

#ifndef WEAPON_TRANSLATOR_H
#define WEAPON_TRANSLATOR_H
#include <map>
#include <string>

#include "weapon.h"


class WeaponTranslator {
    static const std::map<std::string, Weapon>& string_to_code_map();

public:
    static Weapon string_to_code(const std::string& weapon_name);
};


#endif  // WEAPON_TRANSLATOR_H
