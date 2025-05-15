//
// Created by matias on 15/05/25.
//

#ifndef PLAYERDTO_H
#define PLAYERDTO_H

class PlayerDTO {
public:
    const int position_x;
    const int position_y;
    PlayerDTO(const int position_x, const int position_y):
            position_x(position_x), position_y(position_y) {}
};

#endif  // PLAYERDTO_H
