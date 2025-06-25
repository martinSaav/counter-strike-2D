//
// Created by matias on 23/06/25.
//

#include "weapon_translator.h"


const std::map<std::string, Weapon>& WeaponTranslator::string_to_code_map() {
    static const std::map<std::string, Weapon> map = {
            {"ak-47", Weapon::AK47}, {"awp", Weapon::AWP},     {"glock", Weapon::Glock},
            {"m3", Weapon::M3},      {"knife", Weapon::Knife}, {"bomb", Weapon::Bomb}};
    return map;
}


Weapon WeaponTranslator::string_to_code(const std::string& weapon_name) {
    auto& map = string_to_code_map();
    const auto pair = map.find(weapon_name);
    if (pair == map.end()) {
        return Weapon::None;
    }
    return pair->second;
}
