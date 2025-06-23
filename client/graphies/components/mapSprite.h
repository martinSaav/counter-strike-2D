#ifndef MAPSPRITE_H
#define MAPSPRITE_H

#include "component.h"
#include "../../common/dto/game_state_update.h"

class MapSprite : public Component{

    private:
    int anchoField = 2300;
    int altoField = 2300;

    int anchoCartel = 210;
    int altoCartel = 50;

    // Shop
    int altoShopPantalla = 600;
    int anchoShopPantalla = 600;

    bool is_exploding = false;
    int explosion_frame = 0;
    Uint32 explosion_start_time = 0;
    const int explosion_total_frames = 25;
    const int explosion_frame_duration = 100; // milisegundos por frame
    int explosion_x = 0;
    int explosion_y = 0;
    int last_bomb_x = 0;
    int last_bomb_y = 0;


    SDL_Rect destRectMap = {0, 0, 1000, 1000};

    bool is_bomb_activated = false;

    public:
    MapSprite(Renderer* sdlRenderer, Configuracion& configuracion);

    void draw();

    int getWidth();

    int getHeight();

    void drawEndRound(Team& team, int zoom);

    void drawBomb(int bomb_x, int bomb_y);

    bool isBombActivated();

    void activateBomb();

    void exploitBomb();
    
    void drawExplosion();


    void desactivateBomb();

    void drawCampField(int angle, int playerX, int playerY);

    void drawShop();
};
#endif
