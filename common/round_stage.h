#ifndef ROUND_STAGE_H
#define ROUND_STAGE_H

#include <cstdint>

enum class RoundStage : uint8_t {
    Buy = 0x00,
    PrePlant = 0x01,
    PostPlant = 0x02,
    AfterRound = 0x03,
};

#endif
