//
// Created by matias on 08/06/25.
//

#ifndef IMPACT_INFO_H
#define IMPACT_INFO_H
#include <memory>
#include <optional>
#include <utility>

#include "player.h"


class ImpactInfo {
public:
    const bool hit;
    const int distance;
    const std::pair<int, int> impact_position;
    std::optional<std::shared_ptr<Player>> impacted_player;
    ImpactInfo(): hit(false), distance(0), impacted_player(std::nullopt) {}
    ImpactInfo(const int distance, const std::pair<int, int>& impact_position):
            hit(true),
            distance(distance),
            impact_position(impact_position),
            impacted_player(std::nullopt) {}
    ImpactInfo(const int distance, const std::pair<int, int>& impact_position,
               std::shared_ptr<Player> impacted_player):
            hit(true),
            distance(distance),
            impact_position(impact_position),
            impacted_player(std::move(impacted_player)) {}
};


#endif  // IMPACT_INFO_H
