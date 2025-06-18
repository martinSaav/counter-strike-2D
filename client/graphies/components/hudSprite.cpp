#include "hudSprite.h"
#include <cmath>

HudSprite::HudSprite(Renderer* sdlRenderer, Configuracion& configuracion) : 
    Component(sdlRenderer, configuracion){

    texturas.loadTexture("mira", "../client/data/mira.png");
    texturas.loadTexture("symbols", "../client/data/hud/hud_symbols.png");
    texturas.loadTexture("nums", "../client/data/hud/hud_nums.png");
    texturas.loadTexture("nums2", "../client/data/hud/nums2.png");

    // weapons
    texturas.loadTexture("weapons", "../client/data/weapons/weapons.png");
    texturas.loadTexture("bullets", "../client/data/weapons/bullet.png");

    // slot
    texturas.loadTexture("slot", "../client/data/weapons/hud_slot.png");

    // shop
    texturas.loadTexture("shop", "../client/data/maps/shop.png");

    // weapons dropped
    texturas.loadTexture("ak47_dropped", "../client/data/weapons/ak47_d.bmp");
    texturas.loadTexture("glock_dropped", "../client/data/weapons/glock_d.bmp");
    texturas.loadTexture("awp_dropped", "../client/data/weapons/awp_d.bmp");
    texturas.loadTexture("m3_dropped", "../client/data/weapons/m3_d.bmp");
    texturas.loadTexture("bomb_dropped", "../client/data/weapons/bomb_d.bmp");
}

void HudSprite::draw(SDL_Rect& mouse, int& time, int& round, PlayerInfo& myPlayer){
    Texture& mira = texturas.getTexture("mira");
    Texture& hud_symbols = texturas.getTexture("symbols"); 
    Texture& nums = texturas.getTexture("nums");
    HudType tipo = HEALTH;

    int health = myPlayer.get_health();
    int money = myPlayer.get_money();
    Weapon myWeapon = myPlayer.get_active_weapon();
    int myWeaponAmmo = myPlayer.get_active_weapon_ammo();
    Weapon bomb = myPlayer.get_bomb();
    bool haveBomb = false;
    if (bomb == Weapon::Bomb){
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
    drawNumRound(round, symbolX, symbolY);

    symbolX = weidthWindow * 0.05;
    symbolY = heightWindow * 0.92;

    // health
    drawHuds(health, tipo, symbolX, symbolY);

    // Aplicamos el color naranja
    nums.SetColorMod(255, 165, 0);
    hud_symbols.SetColorMod(255, 165, 0);

    if (time < configuracion.tiempoDeCompra){
        // Shop
        symbolX = weidthWindow * 0.28;
        tipo = SHOP;
        int numTipo = tipo;
        drawSymbol(numTipo, symbolX, symbolY, hud_symbols);
    }

    if (haveBomb){
        symbolX = weidthWindow * 0.34;
        tipo = BOMB;
        int numTipo = tipo;
        drawSymbol(numTipo, symbolX, symbolY, hud_symbols);
    }
    
    // time
    symbolX = weidthWindow * 0.45;
    tipo = TIME;
    drawHuds(time, tipo, symbolX, symbolY);

    // weapon
    symbolX = weidthWindow * 0.76;

    int anchoWeapon;
    int weaponTextureX;

    // Coordenadas de la textura
    switch (myWeapon)
    {
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

    drawWeapon(symbolX, symbolY, anchoWeapon, weaponTextureX, myWeaponAmmo, myWeapon);

    // money
    symbolX = weidthWindow * 0.80;
    symbolY = heightWindow * 0.92;
    symbolY -= alturaSymbols;
    tipo = MONEY;
    drawHuds(money, tipo, symbolX, symbolY);

    // mira
    sdlRenderer->Copy(mira, SDL2pp::NullOpt, mouse);

    if (time < configuracion.tiempoDeCompra){
        drawShop();
    }
}

void HudSprite::drawHuds(int num, HudType tipo, int& symbolX, int& symbolY){
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

void HudSprite::drawWeapon(int& symbolX, int& symbolY, int& anchoWeapon,
        int& weaponTextureX, int& ammoWeapon, Weapon& weaponPLayer){
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

    int anchoWeaponsPantalla = anchoWeapon * 2;
    int alturaWeaponsPantalla = alturaWeapons * 2;

    int anchoSlotPantalla = 200;

    int alturaBullets = 50;
    int anchoBullets = 50;

    int alturaBulletsPantalla = 40;
    int anchoBulletsPantalla = 40;

    // Slot
    SDL_Rect destRect = {symbolX, symbolY, anchoSlotPantalla, altoSymbolPantalla};
    sdlRenderer->Copy(slot, SDL2pp::NullOpt, destRect);

    if (weaponPLayer == Weapon::Bomb){
        return;
    }

    // Weapon
    SDL_Rect srcRect = {weaponTextureX, 0, anchoWeapon, alturaWeapons};
    destRect = {symbolX + 5, symbolY + 5, anchoWeaponsPantalla, alturaWeaponsPantalla};
    sdlRenderer->Copy(weapon, srcRect, destRect);

    if (weaponPLayer == Weapon::Knife){
        return;
    }
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
    std::string numStr = std::to_string(ammoWeapon);
    for (char c : numStr){
        int digito = c - '0';

        srcRect = {anchoNums * digito , 0,anchoNums, alturaSymbols};
        destRect = {symbolX, symbolY, anchoNumPantalla, altoNumPantalla};

        sdlRenderer->Copy(numsBullets, srcRect, destRect);

        symbolX += anchoNumPantalla;
    }
}

void HudSprite::drawShop(){
    Texture& shop = texturas.getTexture("shop");

    int altoCartelPantalla = 600;
    int anchoCartelPantalla = 600;

    int symbolX = (configuracion.widthWindow * 0.5) - anchoCartelPantalla / 2;
    int symbolY = configuracion.heightWindow * 0.2;

    SDL_Rect destRect = {
        symbolX,
        symbolY,
        anchoCartelPantalla,
        altoCartelPantalla
    };
    sdlRenderer->Copy(shop, SDL2pp::NullOpt, destRect);
}

void HudSprite::drawWeaponDroped(Weapon weapon, int weaponX, int weaponY){

    int anchoWeapon = 90;
    int altoWeapon = 70;
    std::string weaponDropped;
    switch (weapon)
    {
    case Weapon::AK47:
        weaponDropped = "ak47_dropped";
        break;
    case Weapon::Glock:
        weaponDropped = "glock_dropped";
        anchoWeapon = 70;
        break;
    case Weapon::AWP:
        weaponDropped = "awp_dropped";
        break;
    case Weapon::M3:
        weaponDropped = "m3_dropped";
        break;
    case Weapon::Bomb:
        weaponDropped = "bomb_dropped";
        anchoWeapon = 40;
        altoWeapon = 40;
        break;
    }

    Texture& textureWeapon = texturas.getTexture(weaponDropped);

    SDL_Rect destRect = {
        int((weaponX - configuracion.camera.x) * configuracion.zoom),
        int((weaponY - configuracion.camera.y) * configuracion.zoom),
        anchoWeapon,
        altoWeapon
    };
    sdlRenderer->Copy(textureWeapon, SDL2pp::NullOpt, destRect);
}