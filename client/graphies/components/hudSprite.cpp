#include "hudSprite.h"

#include <cmath>

HudSprite::HudSprite(Renderer* sdlRenderer, Configuracion& configuracion):
        Component(sdlRenderer, configuracion) {

    texturas.loadTexture("mira", "../client/data/mira.png");
    texturas.loadTexture("symbols", "../client/data/hud/hud_symbols.png");
    texturas.loadTexture("nums", "../client/data/hud/hud_nums.png");
    texturas.loadTexture("nums2", "../client/data/hud/nums2.png");

    // weapons
    texturas.loadTexture("weapons", "../client/data/weapons/weapons.png");
    texturas.loadTexture("bullets", "../client/data/weapons/bullet.png");

    // slot
    texturas.loadTexture("slot", "../client/data/weapons/hud_slot.png");

    // posters player
    texturas.loadTexture("counterTerroristPost", "../client/data/hud/counterTerroristPost.png");
    texturas.loadTexture("terroristPost", "../client/data/hud/terroristPost.png");

    // weapons dropped
    texturas.loadTexture("ak47_dropped", "../client/data/weapons/ak47_d.bmp");
    texturas.loadTexture("glock_dropped", "../client/data/weapons/glock_d.bmp");
    texturas.loadTexture("awp_dropped", "../client/data/weapons/awp_d.bmp");
    texturas.loadTexture("m3_dropped", "../client/data/weapons/m3_d.bmp");
    texturas.loadTexture("bomb_dropped", "../client/data/weapons/bomb_d.bmp");

    // Aplico colores
    Texture& counterPost = texturas.getTexture("counterTerroristPost");
    Texture& terroristPost = texturas.getTexture("terroristPost");
    counterPost.SetColorMod(255, 165, 0);
    terroristPost.SetColorMod(255, 165, 0);
}

void HudSprite::draw(SDL_Rect& mouse, int& time, int& round, PlayerInfo& myPlayer,
                     bool& is_bomb_planted) {
    Texture& hud_symbols = texturas.getTexture("symbols");
    Texture& nums = texturas.getTexture("nums");
    HudType tipo = HEALTH;

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

    int weidthWindow = configuracion.widthWindow;
    int heightWindow = configuracion.heightWindow;

    // num round
    int anchoNums = 38;
    int symbolX = (weidthWindow * 0.5) - anchoNums / 2;
    int symbolY = heightWindow * 0.02;

    textureName = "nums2";
    srcRect = {anchoNums * round, 0, anchoNums, alturaSymbols};
    destRect = {symbolX, symbolY, anchoNums, 40};
    drawHud(srcRect, destRect, textureName);

    symbolX = weidthWindow * 0.05;
    symbolY = heightWindow * 0.92;

    // health
    drawHuds(health, tipo, symbolX, symbolY);

    // Aplicamos el color naranja
    nums.SetColorMod(255, 165, 0);
    hud_symbols.SetColorMod(255, 165, 0);

    textureName = "symbols";

    if (haveBomb) {
        symbolX = weidthWindow * 0.34;
        tipo = BOMB;
        int numTipo = tipo;
        srcRect = {anchoSymbols * numTipo, 0, anchoSymbols, alturaSymbols};
        destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
        drawHud(srcRect, destRect, textureName);
    }

    // time
    symbolX = weidthWindow * 0.45;
    tipo = TIME;
    drawHuds(time, tipo, symbolX, symbolY);

    // weapon
    symbolX = weidthWindow * 0.76;

    int anchoWeapon;
    int weaponTextureX;

    castCordinates(myWeapon, anchoWeapon, weaponTextureX);
    drawWeapon(symbolX, symbolY, anchoWeapon, weaponTextureX, myWeaponAmmo, myWeapon);

    // money
    symbolX = weidthWindow * 0.80;
    symbolY -= alturaSymbols;
    tipo = MONEY;
    drawHuds(money, tipo, symbolX, symbolY);

    if (time < configuracion.tiempoDeCompra && !is_bomb_planted) {
        textureName = "symbols";
        // Shop
        symbolY = heightWindow * 0.92;
        symbolX = weidthWindow * 0.28;
        tipo = SHOP;
        int numTipo = tipo;
        srcRect = {anchoSymbols * numTipo, 0, anchoSymbols, alturaSymbols};
        destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
        drawHud(srcRect, destRect, textureName);
    }

    symbolX = weidthWindow * 0.05;
    symbolY = heightWindow * 0.00;
    int anchoCartel;
    int altoCartel = 100;
    // team poster
    char team = myPlayer.get_skin()[0];
    if (team == 'C') {
        textureName = "counterTerroristPost";
        anchoCartel = 260;
    } else {
        textureName = "terroristPost";
        anchoCartel = 200;
    }

    destRect = {symbolX, symbolY, anchoCartel, altoCartel};
    drawHud2(destRect, textureName);

    // mira
    textureName = "mira";
    drawHud2(mouse, textureName);

    drawSounds(myPlayer);
}

void HudSprite::drawHuds(int num, HudType tipo, int& symbolX, int& symbolY) {
    textureName = "symbols";
    int numTipo = tipo;

    // Symbol
    srcRect = {anchoSymbols * numTipo, 0, anchoSymbols, alturaSymbols};
    destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
    drawHud(srcRect, destRect, textureName);

    // Numbers
    textureName = "nums";
    if (tipo == TIME) {

        int minutos = num / 60;
        int segundos = num % 60;
        // Draw time minute
        symbolX += anchoSymbols;
        srcRect = {anchoNum * minutos, 0, anchoNum, alturaNum};
        destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
        drawHud(srcRect, destRect, textureName);

        // Draw ':' symbol
        symbolX += anchoSymbolPantalla;
        numTipo = DOBLEPUNTO;

        srcRect = {anchoNum * numTipo, 0, anchoNum, alturaNum};
        destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
        drawHud(srcRect, destRect, textureName);

        // Draw time seconds
        // Agregamos separacion entre el symbolo y numeros
        int correcionEspacio = 20;
        symbolX -= correcionEspacio;
        num = segundos;

        if (num < 10) {
            int digito = 0;
            symbolX += anchoSymbolPantalla;
            // cppcheck-suppress knownArgument
            srcRect = {anchoNum * digito, 0, anchoNum, alturaNum};
            destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
            drawHud(srcRect, destRect, textureName);
        }
    }

    std::string numStr = std::to_string(num);
    int separacionNums = 0;
    for (char c: numStr) {
        int digito = c - '0';

        symbolX += anchoSymbolPantalla + separacionNums;
        separacionNums = 2;
        srcRect = {anchoNum * digito, 0, anchoNum, alturaNum};
        destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};
        drawHud(srcRect, destRect, textureName);
    }
}

void HudSprite::drawWeapon(int symbolX, int symbolY, int& anchoWeapon, int& weaponTextureX,
                           int& ammoWeapon, Weapon& weaponPLayer) {
    Texture& weapon = texturas.getTexture("weapons");
    Texture& slot = texturas.getTexture("slot");
    Texture& numsBullets = texturas.getTexture("nums2");

    // Aplicamos el color naranja
    weapon.SetColorMod(255, 165, 0);
    slot.SetColorMod(255, 165, 0);
    numsBullets.SetColorMod(255, 165, 0);

    int anchoWeaponsPantalla = anchoWeapon * 2;
    int alturaWeaponsPantalla = alturaWeapons * 2;

    // Slot
    textureName = "slot";
    destRect = {symbolX, symbolY, anchoSlotPantalla, altoSymbolPantalla};
    drawHud2(destRect, textureName);

    if (weaponPLayer == Weapon::Bomb) {
        return;
    }

    // Weapon
    srcRect = {weaponTextureX, 0, anchoWeapon, alturaWeapons};
    destRect = {symbolX + 5, symbolY + 5, anchoWeaponsPantalla, alturaWeaponsPantalla};
    textureName = "weapons";
    drawHud(srcRect, destRect, textureName);

    if (weaponPLayer == Weapon::Knife) {
        return;
    }
    // Dibujo bala
    srcRect = {0, 0, anchoBullets, alturaBullets};
    symbolX += anchoSlotPantalla - anchoBullets;
    destRect = {symbolX, symbolY + 10, anchoBulletsPantalla, alturaBulletsPantalla};
    textureName = "bullets";
    drawHud(srcRect, destRect, textureName);

    symbolX -= anchoBulletNum;
    symbolY += altoBulletNumPantalla;

    std::string numStr = std::to_string(ammoWeapon);
    for (char c: numStr) {
        int digito = c - '0';

        srcRect = {anchoBulletNum * digito, 0, anchoBulletNum, alturaSymbols};
        destRect = {symbolX, symbolY, anchoBulletNumPantalla, altoBulletNumPantalla};

        textureName = "nums2";
        drawHud(srcRect, destRect, textureName);
        symbolX += anchoBulletNumPantalla;
    }
}

void HudSprite::drawWeaponDroped(Weapon weapon, int weaponX, int weaponY) {
    int anchoWeapon;
    int altoWeapon;
    castSizesWeapon(weapon, anchoWeapon, altoWeapon, textureName);

    destRect = {int((weaponX - configuracion.camera.x) * configuracion.zoom),
                int((weaponY - configuracion.camera.y) * configuracion.zoom), anchoWeapon,
                altoWeapon};
    drawHud2(destRect, textureName);
}

void HudSprite::castCordinates(const Weapon& myWeapon, int& anchoWeapon, int& weaponTextureX) {
    // Coordenadas de la textura
    switch (myWeapon) {
        case Weapon::AK47:
            anchoWeapon = 45;
            weaponTextureX = 0;
            break;
        case Weapon::Glock:
            anchoWeapon = 24;
            weaponTextureX = 147;
            break;
        case Weapon::Knife:
            anchoWeapon = 52;
            weaponTextureX = 171;
            break;
        case Weapon::AWP:
            anchoWeapon = 56;
            weaponTextureX = 90;
            break;
        case Weapon::M3:
            anchoWeapon = 46;
            weaponTextureX = 45;
            break;
    }
}

void HudSprite::castSizesWeapon(const Weapon& weapon, int& anchoWeapon, int& altoWeapon,
                                std::string& textureName) {
    anchoWeapon = 90;
    altoWeapon = 70;

    switch (weapon) {
        case Weapon::AK47:
            textureName = "ak47_dropped";
            break;
        case Weapon::Glock:
            textureName = "glock_dropped";
            anchoWeapon = 70;
            break;
        case Weapon::AWP:
            textureName = "awp_dropped";
            break;
        case Weapon::M3:
            textureName = "m3_dropped";
            break;
        case Weapon::Bomb:
            textureName = "bomb_dropped";
            anchoWeapon = 40;
            altoWeapon = 40;
            break;
    }
}

void HudSprite::drawSounds(PlayerInfo& myPlayer) {
    Action actionActual = myPlayer.get_action();

    Weapon weapon = myPlayer.get_active_weapon();
    tipoMusic music;
    int volume = 64;
    switch (actionActual) {
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
