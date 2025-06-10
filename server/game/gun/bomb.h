//
// Created by matias on 09/06/25.
//

#ifndef BOMB_H
#define BOMB_H
#define time_needed_to_plant 3000
#define bomb_dmg 200
#define bomb_hitbox_height 30
#define bomb_hitbox_width 30

class Bomb {
    bool is_planting;
    float time_when_started_to_plant;


public:
    Bomb(): is_planting(false), time_when_started_to_plant(0) {}
    void start_planting(float time);
    void stop_planting();
    [[nodiscard]] bool has_finished_planting(float time) const;
    [[nodiscard]] bool is_currently_planting() const { return is_planting; }
};


#endif  // BOMB_H
