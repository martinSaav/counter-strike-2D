//
// Created by matias on 26/05/25.
//

#ifndef BULLET_H
#define BULLET_H
#include <utility>


class Bullet {
    int bullet_hitbox_width;
    int bullet_hitbox_height;
    std::pair<float, float> position;
    float angle;
    float speed;
    float distance_traveled;
    int minimum_dmg;
    int maximum_dmg;

public:
    Bullet(int bullet_hitbox_width, int bullet_hitbox_height, std::pair<float, float> position,
           float angle, int minimum_dmg, int maximum_dmg);
    void move_to_next_position();
    std::pair<float, float> get_position();
    float calculate_damage();
};


#endif  // BULLET_H
