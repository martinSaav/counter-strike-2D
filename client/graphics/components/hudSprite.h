#ifndef HUDSPRITE_H
#define HUDSPRITE_H

#include <deque>
#include <list>
#include <string>

#include "../../common/dto/kill_info.h"

#include "component.h"

// Una linea del registro de bajas y hasta cuando se muestra (en ticks de SDL)
struct KillFeedEntry {
    KillInfo kill;
    Uint32 shownUntil;
};

enum HudType { HEALTH = 0, TIME = 2, SHOP = 4, BOMB = 6, MONEY = 7, COLON = 10 };

// huds Sobre la pantalla
class HudSprite: public Component {

private:
    int symbolsHeight = 64;
    int symbolsWidth = 64;
    int symbolScreenWidth = 40;
    int symbolScreenHeight = 50;

    int numWidth = 50;
    int numHeight = 66;

    int weaponsHeight = 17;
    int slotScreenWidth = 200;

    // Balas
    int bulletsHeight = 50;
    int bulletsWidth = 50;
    int bulletsScreenHeight = 40;
    int bulletsScreenWidth = 40;

    int bulletNumWidth = 38;
    int bulletNumScreenWidth = 20;
    int bulletNumScreenHeight = 20;

    // Shop
    int shopScreenHeight = 600;
    int shopScreenWidth = 600;

    SDL_Rect srcRect;
    SDL_Rect destRect;

    std::string textureName;

    void drawHuds(int num, HudType type, int& symbolX, int& symbolY);

    void drawWeapon(int symbolX, int symbolY, int& weaponWidth, int& weaponTextureX,
                    int& ammoWeapon, Weapon& weaponPlayer);

    void castCordinates(const Weapon& myWeapon, int& weaponWidth, int& weaponTextureX);

    void castSizesWeapon(const Weapon& myWeapon, int& weaponWidth, int& weaponHeight,
                         std::string& textureName);

    void drawSounds(PlayerInfo& myPlayer);

    // Texto con la fuente de mapa de bits (hud/font.png). height es el alto de cada letra en
    // pixeles; devuelve el ancho dibujado.
    int drawText(const std::string& text, int x, int y, int height, SDL_Color color);

    int textWidth(const std::string& text, int height);

public:
    HudSprite(Renderer* sdlRenderer, Configuration& configuration, TextureStore& textures,
              MusicManager& sounds, const std::string& assetsPath);

    void draw(SDL_Rect& mouse, int& time, int& round, PlayerInfo& myPlayer, bool& is_bomb_planted,
              bool isBuyStage);

    void drawWeaponDroped(Weapon weapon, int weaponX, int weaponY);

    void drawHelp();

    void drawTaskProgress(int progress, bool planting);

    // Ultimas muertes, arriba a la derecha: "matador [arma] victima" con el color de cada equipo
    void drawKillFeed(const std::deque<KillFeedEntry>& feed, const std::list<PlayerInfo>& players);

    // Tabla de puntajes (mientras se mantiene Tab): bajas, muertes y plata por equipo. La plata
    // de los rivales no se muestra.
    void drawScoreboard(const std::list<PlayerInfo>& players, const std::string& myName);
};
#endif
