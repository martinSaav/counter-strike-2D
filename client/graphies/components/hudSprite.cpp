#include "hudSprite.h"
#include <cmath>

HudSprite::HudSprite(Renderer* sdlRenderer, Configuracion& configuracion, int& weidth, int& height) : 
    Component(sdlRenderer, configuracion), weidthScreen(weidth), heightScreen(height){

    texturas.loadTexture("mira", "../client/data/mira.png");
    texturas.loadTexture("symbols", "../client/data/hud/hud_symbols.png");
    texturas.loadTexture("nums", "../client/data/hud/hud_nums.png");
    texturas.loadTexture("nums2", "../client/data/hud/nums2.png");

    // weapons
    texturas.loadTexture("weapons", "../client/data/weapons/weapons.png");
    texturas.loadTexture("bullets", "../client/data/weapons/bullet.png");

    // slot
    texturas.loadTexture("slot", "../client/data/weapons/hud_slot.png");
}

void HudSprite::draw(SDL_Rect& mouse, int& health, int& money, int& time, int& round){
    Texture& mira = texturas.getTexture("mira");
    Texture& hud_symbols = texturas.getTexture("symbols"); 
    Texture& nums = texturas.getTexture("nums");
    HudType tipo = HEALTH;

    // Aplicamos el color verde
    nums.SetColorMod(100, 200, 100);
    hud_symbols.SetColorMod(100, 200, 100);

    // num round
    int anchoNums = 38;
    int symbolX = (weidthScreen * 0.5) - anchoNums / 2;
    int symbolY = heightScreen * 0.02;
    drawNumRound(round, symbolX, symbolY);

    symbolX = weidthScreen * 0.05;
    symbolY = heightScreen * 0.92;

    // health
    drawHuds(health, tipo, symbolX, symbolY);

    // Aplicamos el color naranja
    nums.SetColorMod(255, 165, 0);
    hud_symbols.SetColorMod(255, 165, 0);

    if (time < 45){
        // Shop
        symbolX = weidthScreen * 0.30;
        tipo = SHOP;
        int numTipo = tipo;
        drawSymbol(numTipo, symbolX, symbolY, hud_symbols);
    }
    
    // time
    symbolX = weidthScreen * 0.45;
    tipo = TIME;
    drawHuds(time, tipo, symbolX, symbolY);

    // weapon
    int cantBalas = 20;
    symbolX = weidthScreen * 0.76;
    int anchoWeapons = 45;
    int weaponX = 0;

    drawWeapon(symbolX, symbolY, anchoWeapons, weaponX, cantBalas);

    // money
    symbolX = weidthScreen * 0.80;
    symbolY = heightScreen * 0.92;
    symbolY -= alturaSymbols;
    tipo = MONEY;
    drawHuds(money, tipo, symbolX, symbolY);

    // mira
    sdlRenderer->Copy(mira, SDL2pp::NullOpt, mouse);
}

void HudSprite::drawHuds(int& num, HudType tipo, int& symbolX, int& symbolY){
    Texture& hud_symbols = texturas.getTexture("symbols"); 
    Texture& nums = texturas.getTexture("nums"); 

    int numTipo = tipo;
    // Symbol
    drawSymbol(numTipo, symbolX, symbolY, hud_symbols);

    // Numbers
    if (tipo == 2){

        int minutos = num / 60;
        int segundos = num % 60;
        // Draw time minute
        symbolX += anchoSymbols;
        drawNum(minutos, symbolX, symbolY, nums);

        // Draw ':' symbol
        symbolX += anchoSymbolPantalla;
        numTipo = DOBLEPUNTO;
        drawNum(numTipo, symbolX, symbolY, nums);

        // Draw time seconds
        //Agregamos separacion entre el symbolo y numeros
        int correcionEspacio = 20;
        symbolX -= correcionEspacio;
        num = segundos;
    }

    std::string numStr = std::to_string(num);
    int separacionNums = 0;
    for (char c : numStr){
        int digito = c - '0';

        symbolX += anchoSymbolPantalla + separacionNums;
        separacionNums = 2;
        drawNum(digito, symbolX, symbolY, nums);
    }
}

void HudSprite::drawSymbol(int& posSymbol, int& symbolX, int& symbolY, Texture& hud_symbols){
    SDL_Rect srcRect = {anchoSymbols * posSymbol, 0, anchoSymbols, alturaSymbols};
    SDL_Rect destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};

    sdlRenderer->Copy(hud_symbols, srcRect, destRect);
}

void HudSprite::drawNum(int& num, int& symbolX, int& symbolY, Texture& nums){
    SDL_Rect srcRect = {anchoNum * num, 0, anchoNum, alturaNum};
    SDL_Rect destRect = {symbolX, symbolY, anchoSymbolPantalla, altoSymbolPantalla};

    sdlRenderer->Copy(nums, srcRect, destRect);
}

void HudSprite::drawNumRound(int& round, int& symbolX, int& symbolY){
    Texture& roundNums = texturas.getTexture("nums2");
    int anchoNums = 38;

    SDL_Rect srcRect = {anchoNums * round, 0,anchoNums, alturaSymbols};
    SDL_Rect destRect = {symbolX, symbolY, anchoNums, 40};

    sdlRenderer->Copy(roundNums, srcRect, destRect);
}

void HudSprite::drawWeapon(int& symbolX, int& symbolY, int& anchoWeapons,
        int& weaponX, int& cantBalas){
    Texture& weapon = texturas.getTexture("weapons");
    Texture& slot = texturas.getTexture("slot");
    Texture& bullets = texturas.getTexture("bullets");
    Texture& numsBullets = texturas.getTexture("nums2");

    // Aplicamos el color naranja
    weapon.SetColorMod(255, 165, 0);
    slot.SetColorMod(255, 165, 0);
    numsBullets.SetColorMod(255, 165, 0);

    // default
    int alturaWeapons = 17;

    int anchoWeaponsPantalla = anchoWeapons * 2;
    int alturaWeaponsPantalla = alturaWeapons * 2;

    int anchoSlot = 150;
    int alturaSlot = 40;

    int anchoSlotPantalla = 200;

    int alturaBullets = 50;
    int anchoBullets = 50;

    int alturaBulletsPantalla = 40;
    int anchoBulletsPantalla = 40;

    // Slot
    SDL_Rect srcRect = {0, 0, anchoSlot, alturaSlot};
    SDL_Rect destRect = {symbolX, symbolY, anchoSlotPantalla, altoSymbolPantalla};
    sdlRenderer->Copy(slot, srcRect, destRect);

    // Weapon
    srcRect = {weaponX, 0, anchoWeapons, alturaWeapons};
    destRect = {symbolX + 5, symbolY + 5, anchoWeaponsPantalla, alturaWeaponsPantalla};
    sdlRenderer->Copy(weapon, srcRect, destRect);

    // Dibujo bala
    srcRect = {0, 0, anchoBullets, alturaBullets};
    symbolX += anchoSlotPantalla - anchoBullets;
    destRect = {symbolX, symbolY + 10, anchoBulletsPantalla, alturaBulletsPantalla};
    sdlRenderer->Copy(bullets, srcRect, destRect);

    // Balas
    int anchoNums = 38;
    int anchoNumPantalla = 20;
    int altoNumPantalla = 20;
    symbolX -= anchoNums;
    symbolY += altoNumPantalla;
    
    int separacionNums = 0;
    std::string numStr = std::to_string(cantBalas);
    for (char c : numStr){
        int digito = c - '0';

        srcRect = {anchoNums * digito , 0,anchoNums, alturaSymbols};
        destRect = {symbolX, symbolY, anchoNumPantalla, altoNumPantalla};

        sdlRenderer->Copy(numsBullets, srcRect, destRect);

        symbolX += anchoNumPantalla;
    }
}
