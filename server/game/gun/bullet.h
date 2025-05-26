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
    int minimum_dmg;
    int maximum_dmg;

public:
    Bullet(int bullet_hitbox_width, int bullet_hitbox_height, std::pair<float, float> position,
           float angle, int minimum_dmg, int maximum_dmg);
};


#endif  // BULLET_H
