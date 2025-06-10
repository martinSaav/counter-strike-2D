#ifndef MAPSPRITE_H
#define MAPSPRITE_H

#include "component.h"
#include "../../common/dto/game_state_update.h"

class MapSprite : public Component{

    private:
    SDL_Rect destRectMap = {0, 0, 1000, 1000};

    SDL_Rect srcRectBomb = {0, 0, 32, 32};

    bool is_bomb_activated = false;

    public:
    MapSprite(Renderer* sdlRenderer, Configuracion& configuracion);

    void draw(SDL_Rect& camera ,int& camWidth ,int& camHeight);

    int getWidth();

    int getHeight();

    void drawEndRound(Team& team, int weidthScreen, int heightScreen, int zoom);

    void drawBomb(int bomb_x, int bomb_y);

    bool isBombActivated();

    void activateBomb();

    void desactivateBomb();

    void drawShop(int weidthScreen, int heightScreen);

    void drawCampField(int angle, int playerX, int playerY);
};
#endif
