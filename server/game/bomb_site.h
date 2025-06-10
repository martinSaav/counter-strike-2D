//
// Created by matias on 09/06/25.
//

#ifndef BOMB_SITE_H
#define BOMB_SITE_H

#define bomb_site_height 50
#define bomb_site_width 50

class BombSite {
    int x;
    int y;

public:
    BombSite(const int x, const int y): x(x), y(y) {}
    [[nodiscard]] bool is_position_inside(const int pos_x, const int pos_y) const {
        return pos_x >= x && pos_x <= x + bomb_site_width && pos_y >= y &&
               pos_y <= y + bomb_site_height;
    }
};


#endif  // BOMB_SITE_H
