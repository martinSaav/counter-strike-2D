#ifndef MAPSPRITE_H
#define MAPSPRITE_H

#include <string>

#include "../../common/dto/game_state_update.h"

#include "component.h"

class MapSprite: public Component {

private:
    int anchoField;
    int altoField;

    int anchoCartel = 210;
    int altoCartel = 50;

    // Shop
    int altoShopPantalla = 600;
    int anchoShopPantalla = 600;

    bool is_exploding = false;
    int explosion_frame = 0;
    Uint32 explosion_start_time = 0;
    const int explosion_total_frames = 25;
    const int explosion_frame_duration = 100;  // milisegundos por frame
    int explosion_x = 0;
    int explosion_y = 0;
    int last_bomb_x = 0;
    int last_bomb_y = 0;

    bool is_shaking = false;
    Uint32 shake_start_time = 0;
    int shake_duration_ms = 600;  // duración del efecto
    int shake_magnitude = 4;      // magnitud del efecto


    int canalBomb;

    SDL_Rect destRectMap = {0, 0, 1000, 1000};

    bool is_bomb_activated = false;

    bool is_finish_sound_round = false;

    const MapConfigInfo& mapConfig;

public:
    MapSprite(Renderer* sdlRenderer, Configuracion& configuracion, std::string& assetsPath);

    void draw();

    void drawObstacles();

    void drawBombsite();

    void drawSite(const SiteInfo& site, const std::string& nameSite);

    int getWidth();

    int getHeight();

    void drawEndRound(const Team& team_winer, bool is_bomb_planted);

    void drawBomb(int bomb_x, int bomb_y);

    bool isBombActivated();

    void activateBomb();

    void exploitBomb();

    void drawExplosion();

    bool isShaking() const;

    int getShakeMagnitude() const;

    int getShakeDuration() const;

    Uint32 getShakeStartTime() const;

    void stopShake();

    void desactivateBomb();

    void drawCampField(int angle, int playerX, int playerY);

    void drawShop();
};
#endif
