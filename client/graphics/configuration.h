#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <SDL.h>

#include "../../common/dto/game_config_info.h"

struct Configuration {

    GameConfigInfo* gameConfig;
    float zoom;
    int widthWindow, heightWindow;
    SDL_Rect camera;
    float buyTime;
    int visionOpacity;
    int visionCone;

    Configuration(int width, int height, GameConfigInfo* gameConfig):
            gameConfig(gameConfig),
            zoom(15.0f),
            widthWindow(width),
            heightWindow(height),
            camera{0, 0, int(width / zoom), int(height / zoom)},
            buyTime(gameConfig->get_buy_time() / 1000.0f),
            visionOpacity(gameConfig->get_opacity()),
            visionCone(gameConfig->get_cone_angle()) {}


    void reSizeWindow(int width, int height) {
        widthWindow = width;
        heightWindow = height;

        if (width > 1200 || height > 1200) {
            zoom = 20.0f;
        } else {
            zoom = 15.0f;
        }
    }
};
#endif
