#include "mapSprite.h"


MapSprite::MapSprite(Renderer* sdlRenderer, Configuracion& configuracion, std::string& assetsPath):
        Component(sdlRenderer, configuracion, assetsPath),
        anchoField(0),
        altoField(0),
        canalBomb(0),
        mapConfig(configuracion.gameConfig->get_map_config()) {

    // map
    texturas.loadTexture("map", assetsPath + "/maps/default_aztec.png");

    // map obstacles
    texturas.loadTexture("mapObstacles", assetsPath + "/maps/aztec.bmp");

    // map sites
    texturas.loadTexture("bspot_a", assetsPath + "/maps/bspot_a.png");
    texturas.loadTexture("bspot_b", assetsPath + "/maps/bspot_b.png");

    texturas.loadTexture("cross", assetsPath + "/maps/cross.png");

    // round
    texturas.loadTexture("terroristWins", assetsPath + "/hud/terroristWins.png");
    texturas.loadTexture("counterterroristWins", assetsPath + "/hud/counterTerroristWins.png");

    // bomb
    texturas.loadTexture("bomb", assetsPath + "/weapons/bomb.bmp");

    // camp field
    texturas.loadTexture("field", assetsPath + "/maps/field.png", false);

    // shop
    texturas.loadTexture("shop", assetsPath + "/maps/shop.png");

    // explosion
    texturas.loadTexture("explosion", assetsPath + "/hud/bomb_explotion.png", false);

    tipoMusic music = AMBIENTE;
    int cantVeces = 4;
    sounds.loadMusic(music, cantVeces);

    // Aplico colores
    Texture& counterWins = texturas.getTexture("terroristWins");
    Texture& terroristWins = texturas.getTexture("counterterroristWins");
    counterWins.SetColorMod(255, 165, 0);
    terroristWins.SetColorMod(255, 165, 0);
}

void MapSprite::draw() {
    std::string textureName = "map";
    int camWidth = configuracion.widthWindow / configuracion.zoom;
    int camHeight = configuracion.heightWindow / configuracion.zoom;

    SDL_Rect srcRect = {configuracion.camera.x, configuracion.camera.y, camWidth, camHeight};

    SDL_Rect destRect = {0, 0, configuracion.widthWindow, configuracion.heightWindow};
    drawHud(srcRect, destRect, textureName);

    // Draw bombSite
    drawBombsite();

    // Draw sites
    drawSite(mapConfig.get_ct_site(), "bspot_a");
    drawSite(mapConfig.get_tt_site(), "bspot_b");

    // Draw obstaculos
    drawObstacles();
}

void MapSprite::drawObstacles() {
    std::string textureName = "mapObstacles";
    SDL_Rect srcRect = {480, 64, 32, 32};
    SDL_Rect destRect;
    auto obstacles = mapConfig.get_structures();

    for (auto obstacle: obstacles) {
        int posX = obstacle.get_x() + 1;
        int posY = obstacle.get_y();
        int height = obstacle.get_height();
        int width = obstacle.get_width();

        destRect = {int((posX - configuracion.camera.x) * configuracion.zoom),
                    int((posY - configuracion.camera.y) * configuracion.zoom),
                    int(width * configuracion.zoom), int(height * configuracion.zoom)};
        drawHud(srcRect, destRect, textureName);
    }
}

void MapSprite::drawBombsite() {
    std::string textureName = "cross";
    SDL_Rect srcRect = {192, 32, 32, 32};
    auto bombsite = mapConfig.get_bombsite();

    int bomb_site_height = bombsite.get_height();
    int bomb_site_width = bombsite.get_width();
    int posX = bombsite.get_x();
    int posY = bombsite.get_y();

    SDL_Rect destRect = {int((posX - configuracion.camera.x) * configuracion.zoom),
                         int((posY - configuracion.camera.y) * configuracion.zoom),
                         int(bomb_site_width * configuracion.zoom),
                         int(bomb_site_height * configuracion.zoom)};
    drawHud2(destRect, textureName);
}

void MapSprite::drawSite(const SiteInfo& site, const std::string& nameSite) {
    std::string textureName = nameSite;
    Texture& texture = texturas.getTexture(nameSite);

    // Aplicamos el color rojo
    texture.SetColorMod(255, 0, 0);

    int site_height = site.get_height();
    int site_width = site.get_width();
    int posX = site.get_x();
    int posY = site.get_y();

    SDL_Rect destRect = {int((posX - configuracion.camera.x) * configuracion.zoom),
                         int((posY - configuracion.camera.y) * configuracion.zoom),
                         int(site_width * configuracion.zoom / 2),
                         int(site_height * configuracion.zoom / 2)};
    drawHud2(destRect, textureName);
}

int MapSprite::getWidth() {
    Texture& map = texturas.getTexture("map");
    int worldWidth = map.GetWidth();

    return worldWidth;
}

int MapSprite::getHeight() {
    Texture& map = texturas.getTexture("map");
    int worldHeight = map.GetHeight();

    return worldHeight;
}

void MapSprite::drawEndRound(const Team& team_winer, bool is_bomb_planted) {
    std::string textureName;
    tipoMusic music;
    int anchoCartelPantalla = anchoCartel * configuracion.zoom / 8;
    int symbolX = (configuracion.widthWindow * 0.5) - anchoCartelPantalla / 2;
    int symbolY = configuracion.heightWindow * 0.2;

    SDL_Rect destRect = {symbolX, symbolY, anchoCartelPantalla,
                         int(altoCartel * configuracion.zoom / 4)};

    switch (team_winer) {
        case Team::Terrorists:
            textureName = "terroristWins";
            music = TERRORISTWIN;
            break;
        case Team::CounterTerrorists:
            textureName = "counterterroristWins";
            music = COUNTERTERRORISTWIN;
            break;
    }
    drawHud2(destRect, textureName);


    if (team_winer == Team::CounterTerrorists && is_bomb_activated) {
        music = BOMBHASBEENDEFUSED;
    }
    if (!is_finish_sound_round) {
        sounds.loadSong(music);
        this->is_finish_sound_round = true;
    }
}

bool MapSprite::isBombActivated() { return this->is_bomb_activated; }

void MapSprite::activateBomb() {

    if (is_bomb_activated)
        return;
    this->is_bomb_activated = true;

    tipoMusic music = BOMBHASBEENPLANTED;

    sounds.loadSong(music);

    music = TIMER;
    // Dura 2 segundos
    int cantVeces = 48;
    canalBomb = sounds.loadSong(music, cantVeces);
}

void MapSprite::desactivateBomb() {
    this->is_bomb_activated = false;
    this->is_finish_sound_round = false;
    sounds.stopSongs(canalBomb);
}

void MapSprite::exploitBomb() {
    tipoMusic music = EXPLOIT;
    sounds.loadSong(music);

    is_exploding = true;
    explosion_frame = 0;
    explosion_start_time = SDL_GetTicks();

    explosion_x = last_bomb_x;
    explosion_y = last_bomb_y;
    is_shaking = true;
    shake_start_time =
            SDL_GetTicks() +
            2000;  // retraso para que la explosión se vea antes de que empiece el temblor
    this->is_bomb_activated = false;
}

void MapSprite::drawBomb(int bomb_x, int bomb_y) {

    SDL_Rect destRectBomb = {int((bomb_x - configuracion.camera.x) * configuracion.zoom),
                             int((bomb_y - configuracion.camera.y) * configuracion.zoom),
                             int(32 * configuracion.zoom / 8), int(32 * configuracion.zoom / 8)};

    this->last_bomb_x = bomb_x;
    this->last_bomb_y = bomb_y;

    std::string textureName = "bomb";
    drawHud2(destRectBomb, textureName);
}

void MapSprite::drawExplosion() {
    if (!is_exploding)
        return;

    Uint32 now = SDL_GetTicks();
    Uint32 elapsed = now - explosion_start_time;

    explosion_frame = elapsed / explosion_frame_duration;

    if (explosion_frame >= explosion_total_frames) {
        is_exploding = false;
        return;
    }

    Texture& explosion = texturas.getTexture("explosion");

    int frame_width = 64;
    int frame_height = 64;
    int columns = 5;
    float scale = 3.5f;

    int src_x = (explosion_frame % columns) * frame_width;
    int src_y = (explosion_frame / columns) * frame_height;

    SDL_Rect srcRect = {src_x, src_y, frame_width, frame_height};

    SDL_Rect destRect = {int((explosion_x - configuracion.camera.x) * configuracion.zoom),
                         int((explosion_y - configuracion.camera.y) * configuracion.zoom),
                         int(frame_width * configuracion.zoom * scale / 8),
                         int(frame_height * configuracion.zoom * scale / 8)};

    explosion.SetBlendMode(SDL_BLENDMODE_BLEND);
    sdlRenderer->Copy(explosion, srcRect, destRect);
}


void MapSprite::drawCampField(int angle, int playerX, int playerY) {
    Texture& field = texturas.getTexture("field");

    field.SetBlendMode(SDL_BLENDMODE_BLEND);          // Habilita la mezcla alfa
    field.SetAlphaMod(configuracion.opacidadVision);  // Cambia la opacidad (0-255)

    // coregimos el anglo por la imagen
    angle -= 90;

    // Dimensiones reales de la textura de campo de visión
    int playerCenterX = playerX + int(30 / 8) / 2;
    int playerCenterY = playerY + int(30 / 8) / 2;

    // Posición del jugador en la pantalla
    int playerScreenX = int((playerCenterX - configuracion.camera.x) * configuracion.zoom);
    int playerScreenY = int((playerCenterY - configuracion.camera.y) * configuracion.zoom);

    if (configuracion.heightWindow > 1000 || configuracion.widthWindow > 1000) {
        anchoField = 3700;
        altoField = 3700;
    } else {
        anchoField = 2300;
        altoField = 2300;
    }

    SDL_Rect destRect = {playerScreenX - anchoField / 2, playerScreenY - altoField / 2, anchoField,
                         altoField};
    // Centro de la textura
    SDL_Point center = {anchoField / 2, altoField / 2};

    sdlRenderer->Copy(field, SDL2pp::NullOpt, destRect, angle, center);
}

void MapSprite::drawShop() {
    // Shop
    int symbolX = (configuracion.widthWindow * 0.5) - anchoShopPantalla / 2;
    int symbolY = configuracion.heightWindow * 0.2;
    SDL_Rect destRect = {symbolX, symbolY, anchoShopPantalla, altoShopPantalla};
    std::string textureName = "shop";
    drawHud2(destRect, textureName);
}

bool MapSprite::isShaking() const { return is_shaking; }

int MapSprite::getShakeMagnitude() const { return shake_magnitude; }

int MapSprite::getShakeDuration() const { return shake_duration_ms; }

Uint32 MapSprite::getShakeStartTime() const { return shake_start_time; }

void MapSprite::stopShake() { is_shaking = false; }
