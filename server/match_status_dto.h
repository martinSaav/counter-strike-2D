//
// Created by matias on 15/05/25.
//

#ifndef MATCHSTATUSDTO_H
#define MATCHSTATUSDTO_H
#include <utility>
#include <vector>

#include "server/player_dto.h"

class MatchStatusDTO {
public:
    const std::vector<PlayerDTO> players;
    explicit MatchStatusDTO(std::vector<PlayerDTO>&& players): players(std::move(players)) {}
};

#endif  // MATCHSTATUSDTO_H
