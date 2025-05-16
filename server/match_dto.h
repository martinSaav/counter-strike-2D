//
// Created by matias on 15/05/25.
//

#ifndef MATCH_DTO_H
#define MATCH_DTO_H
#include <string>

class MatchDTO {
public:
    const std::string match_name;
    const int amount_of_players;
    MatchDTO(const std::string& match_name, const int amount_of_players):
            match_name(match_name), amount_of_players(amount_of_players) {}
};

#endif  // MATCH_DTO_H
