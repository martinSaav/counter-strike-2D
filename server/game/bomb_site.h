//
// Created by matias on 09/06/25.
//

#ifndef BOMB_SITE_H
#define BOMB_SITE_H


class BombSite {
public:
    const int bomb_site_height;
    const int bomb_site_width;
    const int x;
    const int y;
    BombSite(const int bomb_site_height, const int bomb_site_width, const int x, const int y):
            bomb_site_height(bomb_site_height), bomb_site_width(bomb_site_width), x(x), y(y) {}
    [[nodiscard]] bool is_position_inside(const int pos_x, const int pos_y) const {
        return pos_x >= x && pos_x <= x + bomb_site_width && pos_y >= y &&
               pos_y <= y + bomb_site_height;
    }
};


#endif  // BOMB_SITE_H
