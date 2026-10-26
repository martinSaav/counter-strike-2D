#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <algorithm>

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
    // Ayuda de controles: se muestra sola en la primera fase de compra y despues se abre y
    // cierra con H.
    bool helpVisible = true;
    bool helpToggled = false;

    Configuration(int width, int height, GameConfigInfo* gameConfig):
            gameConfig(gameConfig),
            zoom(20.0f),
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
            zoom = 26.0f;
        } else {
            zoom = 20.0f;
        }
        camera.w = int(width / zoom);
        camera.h = int(height / zoom);
    }

    void toggleHelp() {
        helpVisible = !helpVisible;
        helpToggled = true;
    }

    // Tamano en pixeles de la textura del campo de vision y radio (en tiles) que cubre: lo que se
    // ve iluminado tiene que coincidir con lo que se considera visible.
    int visionFieldSize() const {
        return (heightWindow > 1000 || widthWindow > 1000) ? 3700 : 2300;
    }

    float visionRadius() const { return visionFieldSize() / 2.0f / zoom; }

    // Donde se dibuja la tienda: un cuadrado que entra en la ventana. Si la ayuda esta abierta
    // (ocupa la mitad izquierda) la tienda se corre a la derecha para que se vean las dos.
    SDL_Rect shopRect() const {
        if (helpVisible) {
            int size = std::min(600, int(std::min(widthWindow * 0.45, heightWindow * 0.7)));
            return SDL_Rect{int(widthWindow * 0.97) - size, int(heightWindow * 0.15), size, size};
        }
        int size = std::min(600, int(std::min(widthWindow, heightWindow) * 0.7));
        return SDL_Rect{widthWindow / 2 - size / 2, int(heightWindow * 0.15), size, size};
    }

    // Donde se dibuja la ayuda (la imagen mide 620x720): mitad izquierda de la ventana.
    SDL_Rect helpRect() const {
        float scale = std::min({1.0f, heightWindow * 0.8f / 720.0f, widthWindow * 0.48f / 620.0f});
        int width = int(620 * scale);
        int height = int(720 * scale);
        return SDL_Rect{int(widthWindow * 0.02), heightWindow / 2 - height / 2, width, height};
    }
};
#endif
