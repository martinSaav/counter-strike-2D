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


const std::map<PlayerSkin, std::string>& SkinTranslator::code_to_string_map() {
    static const std::map<PlayerSkin, std::string> map = {
            {PlayerSkin::C1, "C1"}, {PlayerSkin::C2, "C2"}, {PlayerSkin::C3, "C3"},
            {PlayerSkin::C4, "C4"}, {PlayerSkin::T1, "T1"}, {PlayerSkin::T2, "T2"},
            {PlayerSkin::T3, "T3"}, {PlayerSkin::T4, "T4"}};
    return map;
}

std::map<PlayerSkin, Team> SkinTranslator::skin_to_team_map() {
    static const std::map<PlayerSkin, Team> map = {
            {PlayerSkin::C1, Team::CounterTerrorists}, {PlayerSkin::C2, Team::CounterTerrorists},
            {PlayerSkin::C3, Team::CounterTerrorists}, {PlayerSkin::C4, Team::CounterTerrorists},
            {PlayerSkin::T1, Team::Terrorists},        {PlayerSkin::T2, Team::Terrorists},
            {PlayerSkin::T3, Team::Terrorists},        {PlayerSkin::T4, Team::Terrorists}};
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


std::string SkinTranslator::code_to_string(const PlayerSkin skin) {
    auto map = code_to_string_map();
    return map.find(skin)->second;
}


Team SkinTranslator::get_skin_team(const PlayerSkin skin) {
    auto map = skin_to_team_map();
    const auto it = map.find(skin);
    if (it == map.end()) {
        throw UnknownSkin();
    }
    return it->second;
}
