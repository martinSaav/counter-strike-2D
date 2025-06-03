//
// Created by matias on 03/06/25.
//

#include "skin_translator.h"

const std::map<std::string, PlayerSkin>& SkinTranslator::string_to_code_map() {
    static const std::map<std::string, PlayerSkin> map = {
            {"C1", PlayerSkin::C1}, {"C2", PlayerSkin::C2}, {"C3", PlayerSkin::C3},
            {"C4", PlayerSkin::C4}, {"T1", PlayerSkin::T1}, {"T2", PlayerSkin::T2},
            {"T3", PlayerSkin::T3}, {"T4", PlayerSkin::T4}};
    return map;
}

PlayerSkin SkinTranslator::string_to_code(const std::string& name) {
    auto map = string_to_code_map();
    const auto it = map.find(name);
    if (it == map.end()) {
        throw UnknownSkin();
    }
    return it->second;
}
