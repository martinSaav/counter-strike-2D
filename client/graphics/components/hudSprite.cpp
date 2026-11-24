#include "hudSprite.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <vector>

namespace {

// Grilla de hud/font.png: los caracteres ASCII 32 a 126, 16 por fila (tools/generate_hud_images.py)
constexpr int FONT_COLUMNS = 16;
constexpr int FONT_ROWS = 6;
constexpr int FIRST_CHAR = 32;
constexpr int LAST_CHAR = 126;

constexpr SDL_Color WHITE = {235, 235, 235, 255};
constexpr SDL_Color GRAY = {140, 140, 140, 255};
constexpr SDL_Color ORANGE = {255, 165, 0, 255};
constexpr SDL_Color BLUE = {110, 170, 255, 255};
constexpr SDL_Color HIGHLIGHT = {255, 230, 120, 255};

// La fuente solo tiene ASCII: los bytes de continuacion de UTF-8 se saltean y el resto de lo que
// no entra se muestra como '?' (asi "Martín" queda "Mart?n" y no "Mart??n").
std::string printable(const std::string& text) {
    std::string result;
    for (const char c: text) {
        const auto byte = static_cast<unsigned char>(c);
        if ((byte & 0xC0) == 0x80) {
            continue;
        }
        result += (byte >= FIRST_CHAR && byte <= LAST_CHAR) ? c : '?';
    }
    return result;
}

bool isCounterTerrorist(const PlayerInfo& player) { return player.get_skin()[0] == 'C'; }

// Color del equipo de un jugador (por su skin). Si ya se fue de la partida, blanco.
SDL_Color teamColor(const std::string& name, const std::list<PlayerInfo>& players) {
    const auto player = std::ranges::find_if(
            players, [&name](const PlayerInfo& p) { return p.get_user_name() == name; });
    if (player == players.end()) {
        return WHITE;
    }
    return isCounterTerrorist(*player) ? BLUE : ORANGE;
}

std::string weaponName(Weapon weapon) {
    switch (weapon) {
        case Weapon::Knife:
            return "Cuchillo";
        case Weapon::Glock:
            return "Glock";
        case Weapon::AK47:
            return "AK-47";
        case Weapon::M3:
            return "M3";
        case Weapon::AWP:
            return "AWP";
        case Weapon::Bomb:
            return "Bomba";
        default:
            return "?";
    }
}

}  // namespace

HudSprite::HudSprite(Renderer* sdlRenderer, Configuration& configuration, TextureStore& textures,
                     MusicManager& sounds, const std::string& assetsPath):
        Component(sdlRenderer, configuration, textures, sounds) {

    // HUD básico
    textures.loadTexture("crosshair", assetsPath + "/crosshair.png");
    textures.loadTexture("symbols", assetsPath + "/hud/hud_symbols.png");
    textures.loadTexture("nums", assetsPath + "/hud/hud_nums.png");
    textures.loadTexture("nums2", assetsPath + "/hud/nums2.png");

    // Armas
    textures.loadTexture("weapons", assetsPath + "/weapons/weapons.png");
    textures.loadTexture("bullets", assetsPath + "/weapons/bullet.png");

    // Slot
    textures.loadTexture("slot", assetsPath + "/weapons/hud_slot.png");

    // Posters
    textures.loadTexture("counterTerroristPost", assetsPath + "/hud/counterTerroristPost.png");
    textures.loadTexture("terroristPost", assetsPath + "/hud/terroristPost.png");

    // Armas tiradas
    textures.loadTexture("ak47_dropped", assetsPath + "/weapons/ak47_d.bmp");
    textures.loadTexture("glock_dropped", assetsPath + "/weapons/glock_d.bmp");
    textures.loadTexture("awp_dropped", assetsPath + "/weapons/awp_d.bmp");
    textures.loadTexture("m3_dropped", assetsPath + "/weapons/m3_d.bmp");
    textures.loadTexture("bomb_dropped", assetsPath + "/weapons/bomb_d.bmp");

    // Ayuda y carteles de plantar / desactivar (generados con tools/generate_hud_images.py)
    textures.loadTexture("help", assetsPath + "/hud/help.png", false);
    textures.loadTexture("planting", assetsPath + "/hud/planting.png", false);
    textures.loadTexture("defusing", assetsPath + "/hud/defusing.png", false);
    textures.loadTexture("font", assetsPath + "/hud/font.png", false);

    // Color mod
    Texture& counterPost = textures.getTexture("counterTerroristPost");
    Texture& terroristPost = textures.getTexture("terroristPost");
    counterPost.SetColorMod(255, 165, 0);
    terroristPost.SetColorMod(255, 165, 0);
}


void HudSprite::draw(SDL_Rect& mouse, int& time, int& round, PlayerInfo& myPlayer,
                     bool& is_bomb_planted, bool isBuyStage) {
    Texture& hud_symbols = textures.getTexture("symbols");
    Texture& nums = textures.getTexture("nums");
    HudType type = HEALTH;

    int health = myPlayer.get_health();
    int money = myPlayer.get_money();
    Weapon myWeapon = myPlayer.get_active_weapon();
    int myWeaponAmmo = myPlayer.get_active_weapon_ammo();
    Weapon bomb = myPlayer.get_bomb();
    bool haveBomb = false;
    if (bomb == Weapon::Bomb) {
        haveBomb = true;
    }

    // Aplicamos el color verde
    nums.SetColorMod(100, 200, 100);
    hud_symbols.SetColorMod(100, 200, 100);

    int widthWindow = configuration.widthWindow;
    int heightWindow = configuration.heightWindow;

    // num round
    int numsWidth = 38;
    int symbolX = (widthWindow * 0.5) - numsWidth / 2;
    int symbolY = heightWindow * 0.02;

    textureName = "nums2";
    srcRect = {numsWidth * round, 0, numsWidth, symbolsHeight};
    destRect = {symbolX, symbolY, numsWidth, 40};
    drawHud(srcRect, destRect, textureName);

    symbolX = widthWindow * 0.05;
    symbolY = heightWindow * 0.92;

    // health
    drawHuds(health, type, symbolX, symbolY);

    // Aplicamos el color naranja
    nums.SetColorMod(255, 165, 0);
    hud_symbols.SetColorMod(255, 165, 0);

    textureName = "symbols";

    if (haveBomb) {
        symbolX = widthWindow * 0.34;
        type = BOMB;
        int typeNum = type;
        srcRect = {symbolsWidth * typeNum, 0, symbolsWidth, symbolsHeight};
        destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
        drawHud(srcRect, destRect, textureName);
    }

    // time: con la bomba plantada el reloj pasa a ser la cuenta regresiva de la detonacion,
    // en rojo y con el simbolo de la bomba
    symbolX = widthWindow * 0.45;
    type = TIME;
    if (is_bomb_planted) {
        nums.SetColorMod(230, 40, 40);
        hud_symbols.SetColorMod(230, 40, 40);
        type = BOMB;
    }
    drawHuds(time, type, symbolX, symbolY);
    nums.SetColorMod(255, 165, 0);
    hud_symbols.SetColorMod(255, 165, 0);

    // weapon
    symbolX = widthWindow * 0.76;

    int weaponWidth;
    int weaponTextureX;

    castCordinates(myWeapon, weaponWidth, weaponTextureX);
    drawWeapon(symbolX, symbolY, weaponWidth, weaponTextureX, myWeaponAmmo, myWeapon);

    // money
    symbolX = widthWindow * 0.80;
    symbolY -= symbolsHeight;
    type = MONEY;
    drawHuds(money, type, symbolX, symbolY);

    if (isBuyStage) {
        textureName = "symbols";
        // Shop
        symbolY = heightWindow * 0.92;
        symbolX = widthWindow * 0.28;
        type = SHOP;
        int typeNum = type;
        srcRect = {symbolsWidth * typeNum, 0, symbolsWidth, symbolsHeight};
        destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
        drawHud(srcRect, destRect, textureName);
    }

    symbolX = widthWindow * 0.05;
    symbolY = heightWindow * 0.00;
    int bannerWidth;
    int bannerHeight = 100;
    // team poster
    char team = myPlayer.get_skin()[0];
    if (team == 'C') {
        textureName = "counterTerroristPost";
        bannerWidth = 260;
    } else {
        textureName = "terroristPost";
        bannerWidth = 200;
    }

    destRect = {symbolX, symbolY, bannerWidth, bannerHeight};
    drawHud2(destRect, textureName);

    // mira
    textureName = "crosshair";
    drawHud2(mouse, textureName);

    drawSounds(myPlayer);
}

void HudSprite::drawHuds(int num, HudType type, int& symbolX, int& symbolY) {
    textureName = "symbols";
    int typeNum = type;

    // Symbol
    srcRect = {symbolsWidth * typeNum, 0, symbolsWidth, symbolsHeight};
    destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
    drawHud(srcRect, destRect, textureName);

    // Numbers
    textureName = "nums";
    if (type == TIME || type == BOMB) {

        int minutes = num / 60;
        int seconds = num % 60;
        // Draw time minute
        symbolX += symbolsWidth;
        srcRect = {numWidth * minutes, 0, numWidth, numHeight};
        destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
        drawHud(srcRect, destRect, textureName);

        // Draw ':' symbol
        symbolX += symbolScreenWidth;
        typeNum = COLON;

        srcRect = {numWidth * typeNum, 0, numWidth, numHeight};
        destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
        drawHud(srcRect, destRect, textureName);

        // Draw time seconds
        // Agregamos separacion entre el symbolo y numeros
        int spaceCorrection = 20;
        symbolX -= spaceCorrection;
        num = seconds;

        if (num < 10) {
            int digit = 0;
            symbolX += symbolScreenWidth;
            // cppcheck-suppress knownArgument
            srcRect = {numWidth * digit, 0, numWidth, numHeight};
            destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
            drawHud(srcRect, destRect, textureName);
        }
    }

    std::string numStr = std::to_string(num);
    int numsSeparation = 0;
    for (char c: numStr) {
        int digit = c - '0';

        symbolX += symbolScreenWidth + numsSeparation;
        numsSeparation = 2;
        srcRect = {numWidth * digit, 0, numWidth, numHeight};
        destRect = {symbolX, symbolY, symbolScreenWidth, symbolScreenHeight};
        drawHud(srcRect, destRect, textureName);
    }
}

void HudSprite::drawWeapon(int symbolX, int symbolY, int& weaponWidth, int& weaponTextureX,
                           int& ammoWeapon, Weapon& weaponPlayer) {
    Texture& weapon = textures.getTexture("weapons");
    Texture& slot = textures.getTexture("slot");
    Texture& numsBullets = textures.getTexture("nums2");

    // Aplicamos el color naranja
    weapon.SetColorMod(255, 165, 0);
    slot.SetColorMod(255, 165, 0);
    numsBullets.SetColorMod(255, 165, 0);

    int weaponsScreenWidth = weaponWidth * 2;
    int weaponsScreenHeight = weaponsHeight * 2;

    // Slot
    textureName = "slot";
    destRect = {symbolX, symbolY, slotScreenWidth, symbolScreenHeight};
    drawHud2(destRect, textureName);

    if (weaponPlayer == Weapon::Bomb) {
        return;
    }

    // Weapon
    srcRect = {weaponTextureX, 0, weaponWidth, weaponsHeight};
    destRect = {symbolX + 5, symbolY + 5, weaponsScreenWidth, weaponsScreenHeight};
    textureName = "weapons";
    drawHud(srcRect, destRect, textureName);

    if (weaponPlayer == Weapon::Knife) {
        return;
    }
    // Dibujo bala
    srcRect = {0, 0, bulletsWidth, bulletsHeight};
    symbolX += slotScreenWidth - bulletsWidth;
    destRect = {symbolX, symbolY + 10, bulletsScreenWidth, bulletsScreenHeight};
    textureName = "bullets";
    drawHud(srcRect, destRect, textureName);

    symbolX -= bulletNumWidth;
    symbolY += bulletNumScreenHeight;

    std::string numStr = std::to_string(ammoWeapon);
    for (char c: numStr) {
        int digit = c - '0';

        srcRect = {bulletNumWidth * digit, 0, bulletNumWidth, symbolsHeight};
        destRect = {symbolX, symbolY, bulletNumScreenWidth, bulletNumScreenHeight};

        textureName = "nums2";
        drawHud(srcRect, destRect, textureName);
        symbolX += bulletNumScreenWidth;
    }
}

void HudSprite::drawWeaponDroped(Weapon weapon, int weaponX, int weaponY) {
    int weaponWidth;
    int weaponHeight;
    castSizesWeapon(weapon, weaponWidth, weaponHeight, textureName);

    destRect = {int((weaponX - configuration.camera.x) * configuration.zoom),
                int((weaponY - configuration.camera.y) * configuration.zoom), weaponWidth,
                weaponHeight};
    drawHud2(destRect, textureName);
}

void HudSprite::castCordinates(const Weapon& myWeapon, int& weaponWidth, int& weaponTextureX) {
    // Coordenadas de la textura
    switch (myWeapon) {
        case Weapon::AK47:
            weaponWidth = 45;
            weaponTextureX = 0;
            break;
        case Weapon::Glock:
            weaponWidth = 24;
            weaponTextureX = 147;
            break;
        case Weapon::Knife:
            weaponWidth = 52;
            weaponTextureX = 171;
            break;
        case Weapon::AWP:
            weaponWidth = 56;
            weaponTextureX = 90;
            break;
        case Weapon::M3:
            weaponWidth = 46;
            weaponTextureX = 45;
            break;
    }
}

void HudSprite::castSizesWeapon(const Weapon& weapon, int& weaponWidth, int& weaponHeight,
                                std::string& textureName) {
    weaponWidth = 90;
    weaponHeight = 70;

    switch (weapon) {
        case Weapon::AK47:
            textureName = "ak47_dropped";
            break;
        case Weapon::Glock:
            textureName = "glock_dropped";
            weaponWidth = 70;
            break;
        case Weapon::AWP:
            textureName = "awp_dropped";
            break;
        case Weapon::M3:
            textureName = "m3_dropped";
            break;
        case Weapon::Bomb:
            textureName = "bomb_dropped";
            weaponWidth = 40;
            weaponHeight = 40;
            break;
    }
}

void HudSprite::drawSounds(PlayerInfo& myPlayer) {
    Action currentAction = myPlayer.get_action();

    Weapon weapon = myPlayer.get_active_weapon();
    MusicType music;
    int volume = 64;
    switch (currentAction) {
        case Action::Reload:

            volume = 50;
            switch (weapon) {
                case Weapon::Knife:
                    return;
                case Weapon::Glock:
                    music = PISTOLRELOAD;
                    break;
                default:
                    // Rifles
                    music = RIFLERELOAD;
                    break;
            }
            break;
        case Action::BuyingWeapon:
            music = BUY;
            break;
        case Action::BuyingAmmo:
            music = BUY;
            break;
        case Action::EquipWeapon:
            volume = 80;
            music = ITEMEQUIP;
            break;
        case Action::PlantBomb:
            music = PLANTINGBOMB;
            break;
        default:
            return;
    }
    sounds.loadSong(music, 0, volume);
}

void HudSprite::drawHelp() {
    textureName = "help";
    drawHud2(configuration.helpRect(), textureName);
}

void HudSprite::drawTaskProgress(int progress, bool planting) {
    // Barra con el avance de la bomba que se esta plantando o desactivando
    int barWidth = int(configuration.widthWindow * 0.3);
    int barHeight = 18;
    int barX = configuration.widthWindow / 2 - barWidth / 2;
    int barY = int(configuration.heightWindow * 0.72);

    textureName = planting ? "planting" : "defusing";
    SDL_Rect labelRect = {configuration.widthWindow / 2 - 180, barY - 44, 360, 40};
    drawHud2(labelRect, textureName);

    sdlRenderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    sdlRenderer->SetDrawColor(0, 0, 0, 180);
    sdlRenderer->FillRect(SDL_Rect{barX, barY, barWidth, barHeight});
    sdlRenderer->SetDrawColor(255, 165, 0, 230);
    sdlRenderer->FillRect(
            SDL_Rect{barX + 2, barY + 2, (barWidth - 4) * progress / 100, barHeight - 4});
}


int HudSprite::textWidth(const std::string& text, int height) {
    Texture& font = textures.getTexture("font");
    const int cellWidth = font.GetWidth() / FONT_COLUMNS;
    const int cellHeight = font.GetHeight() / FONT_ROWS;
    return static_cast<int>(printable(text).size()) * (cellWidth * height / cellHeight);
}

int HudSprite::drawText(const std::string& text, int x, int y, int height, SDL_Color color) {
    Texture& font = textures.getTexture("font");
    const int cellWidth = font.GetWidth() / FONT_COLUMNS;
    const int cellHeight = font.GetHeight() / FONT_ROWS;
    const int glyphWidth = cellWidth * height / cellHeight;
    // Las letras son blancas: el color se lo da el modulado de la textura
    font.SetColorMod(color.r, color.g, color.b);
    int cursor = x;
    for (const char c: printable(text)) {
        const int index = static_cast<unsigned char>(c) - FIRST_CHAR;
        SDL_Rect src = {(index % FONT_COLUMNS) * cellWidth, (index / FONT_COLUMNS) * cellHeight,
                        cellWidth, cellHeight};
        SDL_Rect dst = {cursor, y, glyphWidth, height};
        sdlRenderer->Copy(font, src, dst);
        cursor += glyphWidth;
    }
    return cursor - x;
}

void HudSprite::drawKillFeed(const std::deque<KillFeedEntry>& feed,
                             const std::list<PlayerInfo>& players) {
    const int height = 20;
    const int padding = 6;
    const int margin = 12;
    int y = int(configuration.heightWindow * 0.07);
    for (const auto& entry: feed) {
        const KillInfo& kill = entry.kill;
        // Si la mato la bomba no hay matador: queda "[Bomba] victima"
        const std::string weapon = "[" + weaponName(kill.get_weapon()) + "]";
        const std::string killer = kill.get_killer().empty() ? "" : kill.get_killer() + " ";
        const std::string victim = " " + kill.get_victim();
        const int width = textWidth(killer + weapon + victim, height);
        int x = configuration.widthWindow - margin - width;

        SDL_Rect background = {x - padding, y - padding / 2, width + 2 * padding, height + padding};
        sdlRenderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        sdlRenderer->SetDrawColor(0, 0, 0, 150);
        sdlRenderer->FillRect(background);

        x += drawText(killer, x, y, height, teamColor(kill.get_killer(), players));
        x += drawText(weapon, x, y, height, WHITE);
        drawText(victim, x, y, height, teamColor(kill.get_victim(), players));
        y += height + padding + 2;
    }
}

void HudSprite::drawScoreboard(const std::list<PlayerInfo>& players, const std::string& myName) {
    std::vector<const PlayerInfo*> terrorists;
    std::vector<const PlayerInfo*> counterTerrorists;
    bool iAmCounterTerrorist = false;
    for (const auto& player: players) {
        (isCounterTerrorist(player) ? counterTerrorists : terrorists).push_back(&player);
        if (player.get_user_name() == myName) {
            iAmCounterTerrorist = isCounterTerrorist(player);
        }
    }
    // Mas bajas arriba; a igualdad, menos muertes
    const auto byScore = [](const PlayerInfo* a, const PlayerInfo* b) {
        if (a->get_kills() != b->get_kills()) {
            return a->get_kills() > b->get_kills();
        }
        return a->get_deaths() < b->get_deaths();
    };
    std::ranges::sort(terrorists, byScore);
    std::ranges::sort(counterTerrorists, byScore);

    const int textHeight = 20;
    const int rowHeight = 28;
    const int width = std::min(720, int(configuration.widthWindow * 0.9));
    // Titulo, y por equipo: nombre del equipo, encabezados de columna y una fila por jugador
    const int rows = 2 + 2 * 2 + static_cast<int>(players.size()) + 1;
    const int height = rows * rowHeight;
    const int left = (configuration.widthWindow - width) / 2;
    int y = (configuration.heightWindow - height) / 2;

    SDL_Rect panel = {left, y, width, height};
    sdlRenderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    sdlRenderer->SetDrawColor(15, 25, 15, 240);  // casi opaco: puede quedar encima de la ayuda
    sdlRenderer->FillRect(panel);
    sdlRenderer->SetDrawColor(ORANGE.r, ORANGE.g, ORANGE.b, 255);
    sdlRenderer->DrawRect(panel);

    const std::string title = "PUNTAJES";
    drawText(title, left + (width - textWidth(title, 26)) / 2, y + 10, 26, ORANGE);
    y += 2 * rowHeight;

    const int nameX = left + 20;
    const int killsX = left + int(width * 0.55);
    const int deathsX = left + int(width * 0.69);
    const int moneyX = left + int(width * 0.84);
    const auto drawTeam = [&](const std::string& teamName, SDL_Color color,
                              const std::vector<const PlayerInfo*>& team, bool showMoney) {
        drawText(teamName, nameX, y, textHeight, color);
        y += rowHeight;
        drawText("Bajas", killsX, y, textHeight - 4, GRAY);
        drawText("Muertes", deathsX, y, textHeight - 4, GRAY);
        drawText("Plata", moneyX, y, textHeight - 4, GRAY);
        y += rowHeight;
        for (const PlayerInfo* player: team) {
            SDL_Color rowColor = WHITE;
            if (player->get_status() == Status::Dead) {
                rowColor = GRAY;
            }
            if (player->get_user_name() == myName) {
                rowColor = HIGHLIGHT;
            }
            drawText(player->get_user_name(), nameX + 12, y, textHeight, rowColor);
            drawText(std::to_string(player->get_kills()), killsX, y, textHeight, rowColor);
            drawText(std::to_string(player->get_deaths()), deathsX, y, textHeight, rowColor);
            const std::string money =
                    showMoney ? "$" + std::to_string(player->get_money()) : std::string("-");
            drawText(money, moneyX, y, textHeight, rowColor);
            y += rowHeight;
        }
    };
    drawTeam("TERRORISTAS", ORANGE, terrorists, !iAmCounterTerrorist);
    y += rowHeight / 2;
    drawTeam("ANTITERRORISTAS", BLUE, counterTerrorists, iAmCounterTerrorist);
}
