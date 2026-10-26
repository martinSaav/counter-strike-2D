#include "mapSprite.h"

#include <algorithm>
#include <fstream>


MapSprite::MapSprite(Renderer* sdlRenderer, Configuration& configuration, std::string& assetsPath):
        Component(sdlRenderer, configuration, assetsPath),
        fieldWidth(0),
        fieldHeight(0),
        bombChannel(0),
        mapConfig(configuration.gameConfig->get_map_config()) {

    // map: piso y estructuras segun el tema del mapa (si falta la carpeta, el del desierto)
    std::string themePath = assetsPath + "/maps/" + mapConfig.get_theme();
    if (!std::ifstream(themePath + "/floor.png").good()) {
        themePath = assetsPath + "/maps/desert";
    }
    textures.loadTexture("map", themePath + "/floor.png", false);

    // map obstacles
    textures.loadTexture("mapObstacles", themePath + "/wall.png", false);

    // map sites
    textures.loadTexture("bspot_a", assetsPath + "/maps/bspot_a.png");
    textures.loadTexture("bspot_b", assetsPath + "/maps/bspot_b.png");

    textures.loadTexture("cross", assetsPath + "/maps/cross.png");

    // round
    textures.loadTexture("terroristWins", assetsPath + "/hud/terroristWins.png");
    textures.loadTexture("counterterroristWins", assetsPath + "/hud/counterTerroristWins.png");

    // bomb
    textures.loadTexture("bomb", assetsPath + "/weapons/bomb.bmp");

    // camp field
    textures.loadTexture("field", assetsPath + "/maps/field.png", false);

    // shop
    textures.loadTexture("shop", assetsPath + "/maps/shop.png");

    // explosion
    textures.loadTexture("explosion", assetsPath + "/hud/bomb_explotion.png", false);

    MusicType music = AMBIENCE;
    int times = 4;
    sounds.loadMusic(music, times);

    // Aplico colores
    Texture& counterWins = textures.getTexture("terroristWins");
    Texture& terroristWins = textures.getTexture("counterterroristWins");
    counterWins.SetColorMod(255, 165, 0);
    terroristWins.SetColorMod(255, 165, 0);
}

void MapSprite::drawTiled(Texture& texture, float areaX, float areaY, float areaWidth,
                          float areaHeight) {
    // Repite la textura sobre el area (en tiles del mapa) a escala fija: TEXTURE_PX_PER_TILE
    // pixeles de textura por tile, asi un tile de 32 px del CS2D ocupa lo mismo que un jugador.
    // Solo se dibujan los bloques que entran en la camara, recortando los del borde del area.
    const float blockWidth = static_cast<float>(texture.GetWidth()) / TEXTURE_PX_PER_TILE;
    const float blockHeight = static_cast<float>(texture.GetHeight()) / TEXTURE_PX_PER_TILE;
    const SDL_Rect& camera = configuration.camera;
    const float zoom = configuration.zoom;
    const float viewRight = camera.x + configuration.widthWindow / zoom;
    const float viewBottom = camera.y + configuration.heightWindow / zoom;

    for (float by = areaY; by < areaY + areaHeight && by < viewBottom; by += blockHeight) {
        const float h = std::min(blockHeight, areaY + areaHeight - by);
        if (by + h < camera.y) {
            continue;
        }
        for (float bx = areaX; bx < areaX + areaWidth && bx < viewRight; bx += blockWidth) {
            const float w = std::min(blockWidth, areaX + areaWidth - bx);
            if (bx + w < camera.x) {
                continue;
            }
            SDL_Rect src = {0, 0, int(w * TEXTURE_PX_PER_TILE), int(h * TEXTURE_PX_PER_TILE)};
            // El +1 evita que queden lineas entre bloques por el redondeo
            SDL_Rect dest = {int((bx - camera.x) * zoom), int((by - camera.y) * zoom),
                             int(w * zoom) + 1, int(h * zoom) + 1};
            sdlRenderer->Copy(texture, src, dest);
        }
    }
}

void MapSprite::draw() {
    // Piso del terreno en mosaico sobre todo el mapa
    drawTiled(textures.getTexture("map"), 0, 0, mapConfig.get_map_width(),
              mapConfig.get_map_height());

    // Draw bombSite
    drawBombsite();

    // Draw sites
    drawSite(mapConfig.get_ct_site(), "bspot_a");
    drawSite(mapConfig.get_tt_site(), "bspot_b");

    // Draw obstaculos
    drawObstacles();
}

void MapSprite::drawObstacles() {
    // Estructuras solidas: el tile de pared del terreno en mosaico
    Texture& wall = textures.getTexture("mapObstacles");
    for (const auto& obstacle: mapConfig.get_structures()) {
        drawTiled(wall, obstacle.get_x() + 1, obstacle.get_y(), obstacle.get_width(),
                  obstacle.get_height());
    }
}

void MapSprite::drawBombsite() {
    // Una cruz por cada zona donde se puede plantar la bomba
    std::string textureName = "cross";
    for (const auto& bombsite: mapConfig.get_bombsites()) {
        SDL_Rect destRect = {int((bombsite.get_x() - configuration.camera.x) * configuration.zoom),
                             int((bombsite.get_y() - configuration.camera.y) * configuration.zoom),
                             int(bombsite.get_width() * configuration.zoom),
                             int(bombsite.get_height() * configuration.zoom)};
        drawHud2(destRect, textureName);
    }
}

void MapSprite::drawSite(const SiteInfo& site, const std::string& nameSite) {
    std::string textureName = nameSite;
    Texture& texture = textures.getTexture(nameSite);

    // Aplicamos el color rojo
    texture.SetColorMod(255, 0, 0);

    int site_height = site.get_height();
    int site_width = site.get_width();
    int posX = site.get_x();
    int posY = site.get_y();

    SDL_Rect destRect = {int((posX - configuration.camera.x) * configuration.zoom),
                         int((posY - configuration.camera.y) * configuration.zoom),
                         int(site_width * configuration.zoom / 2),
                         int(site_height * configuration.zoom / 2)};
    drawHud2(destRect, textureName);
}

// Tamano del mundo en tiles (lo usa la camara para no salirse del mapa)
int MapSprite::getWidth() { return mapConfig.get_map_width(); }

int MapSprite::getHeight() { return mapConfig.get_map_height(); }

void MapSprite::drawEndRound(const Team& team_winer, bool is_bomb_planted) {
    std::string textureName;
    MusicType music;
    int bannerScreenWidth = bannerWidth * configuration.zoom / 8;
    int symbolX = (configuration.widthWindow * 0.5) - bannerScreenWidth / 2;
    int symbolY = configuration.heightWindow * 0.2;

    SDL_Rect destRect = {symbolX, symbolY, bannerScreenWidth,
                         int(bannerHeight * configuration.zoom / 4)};

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

    MusicType music = BOMBHASBEENPLANTED;

    sounds.loadSong(music);

    music = TIMER;
    // Dura 2 segundos
    int times = 48;
    bombChannel = sounds.loadSong(music, times);
}

void MapSprite::desactivateBomb() {
    this->is_bomb_activated = false;
    this->is_finish_sound_round = false;
    sounds.stopSongs(bombChannel);
}

void MapSprite::exploitBomb() {
    MusicType music = EXPLOIT;
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

    SDL_Rect destRectBomb = {int((bomb_x - configuration.camera.x) * configuration.zoom),
                             int((bomb_y - configuration.camera.y) * configuration.zoom),
                             int(32 * configuration.zoom / 8), int(32 * configuration.zoom / 8)};

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

    Texture& explosion = textures.getTexture("explosion");

    int frame_width = 64;
    int frame_height = 64;
    int columns = 5;
    float scale = 3.5f;

    int src_x = (explosion_frame % columns) * frame_width;
    int src_y = (explosion_frame / columns) * frame_height;

    SDL_Rect srcRect = {src_x, src_y, frame_width, frame_height};

    SDL_Rect destRect = {int((explosion_x - configuration.camera.x) * configuration.zoom),
                         int((explosion_y - configuration.camera.y) * configuration.zoom),
                         int(frame_width * configuration.zoom * scale / 8),
                         int(frame_height * configuration.zoom * scale / 8)};

    explosion.SetBlendMode(SDL_BLENDMODE_BLEND);
    sdlRenderer->Copy(explosion, srcRect, destRect);
}


void MapSprite::drawCampField(int angle, int playerX, int playerY) {
    Texture& field = textures.getTexture("field");

    field.SetBlendMode(SDL_BLENDMODE_BLEND);         // Habilita la mezcla alfa
    field.SetAlphaMod(configuration.visionOpacity);  // Cambia la opacidad (0-255)

    // coregimos el anglo por la imagen
    angle -= 90;

    // El sprite del jugador mide 32 / 8 = 4 tiles: el cono sale de su centro
    int playerCenterX = playerX + 2;
    int playerCenterY = playerY + 2;

    // Posición del jugador en la pantalla
    int playerScreenX = int((playerCenterX - configuration.camera.x) * configuration.zoom);
    int playerScreenY = int((playerCenterY - configuration.camera.y) * configuration.zoom);

    fieldWidth = configuration.visionFieldSize();
    fieldHeight = configuration.visionFieldSize();

    SDL_Rect destRect = {playerScreenX - fieldWidth / 2, playerScreenY - fieldHeight / 2,
                         fieldWidth, fieldHeight};
    // Centro de la textura
    SDL_Point center = {fieldWidth / 2, fieldHeight / 2};

    sdlRenderer->Copy(field, SDL2pp::NullOpt, destRect, angle, center);
}

void MapSprite::drawShop() {
    // Shop (se puede usar con el mouse: ver InputHandler::clickShop)
    SDL_Rect destRect = configuration.shopRect();
    std::string textureName = "shop";
    drawHud2(destRect, textureName);
}

bool MapSprite::isShaking() const { return is_shaking; }

int MapSprite::getShakeMagnitude() const { return shake_magnitude; }

int MapSprite::getShakeDuration() const { return shake_duration_ms; }

Uint32 MapSprite::getShakeStartTime() const { return shake_start_time; }

void MapSprite::stopShake() { is_shaking = false; }
