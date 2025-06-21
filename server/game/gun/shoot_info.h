//
// Created by matias on 21/06/25.
//

#ifndef SHOOT_INFO_H
#define SHOOT_INFO_H

#include <utility>
#include <vector>

#include "shoot_result.h"

class ShootInfo {
public:
    const std::vector<ShootResult> results;
    explicit ShootInfo(std::vector<ShootResult>&& results): results(std::move(results)) {}
};


#endif  // SHOOT_INFO_H
