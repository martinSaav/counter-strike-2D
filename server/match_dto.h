//
// Created by matias on 15/05/25.
//

#ifndef MATCH_DTO_H
#define MATCH_DTO_H
#include <string>
#include <utility>
#define generic_map_name "test_map"
class MatchDTO {
public:
    const std::string match_name;
    const std::string map_name;
    const int number_of_players;
    MatchDTO(std::string match_name, const int number_of_players):
            match_name(std::move(match_name)),
            map_name(generic_map_name),
            number_of_players(number_of_players) {}
};

#endif  // MATCH_DTO_H
