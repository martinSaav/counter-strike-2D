//
// Created by matias on 15/05/25.
//

#ifndef MATCH_DTO_H
#define MATCH_DTO_H
#include <string>
#include <utility>

class MatchDTO {
public:
    const std::string match_name;
    const std::string map_name;
    const int number_of_players;
    const int max_players;
    MatchDTO(std::string match_name, std::string map_name, const int number_of_players,
             const int max_players):
            match_name(std::move(match_name)),
            map_name(std::move(map_name)),
            number_of_players(number_of_players),
            max_players(max_players) {}
};

#endif  // MATCH_DTO_H
