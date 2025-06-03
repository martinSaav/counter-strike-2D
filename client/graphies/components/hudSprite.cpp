#include "hudSprite.h"

HudSprite::HudSprite(Renderer* sdlRenderer, int& weidth, int& height) : 
    Component(sdlRenderer), weidthScreen(weidth), heightScreen(height){

    texturas.loadTexture("mira", "../client/data/mira.png");
    texturas.loadTexture("symbols", "../client/data/hud/hud_symbols.bmp");
    texturas.loadTexture("nums", "../client/data/hud/hud_nums.bmp");
    texturas.loadTexture("roundNumbers", "../client/data/hud/roundNumbers.png");
}

void HudSprite::draw(SDL_Rect& mouse, int& health, int& money, int& time, int& round){
    Texture& mira = texturas.getTexture("mira");
    Texture& hud_symbols = texturas.getTexture("symbols"); 
    Texture& nums = texturas.getTexture("nums");
    Texture& roundNums = texturas.getTexture("roundNumbers");

    // Aplicamos el color verde
    nums.SetColorMod(100, 200, 100);
    hud_symbols.SetColorMod(100, 200, 100);

    int anchoNum = 40;
    int alturaSymbols = 60;
    int anchoSymbols = 64;

    int symbolX = weidthScreen * 0.05;
    int symbolY = heightScreen * 0.92;
    int healthPosSymbol = 0;

    // Symbol health
    drawSymbols(health, healthPosSymbol, symbolX, symbolY);

    // Aplicamos el color naranja
    nums.SetColorMod(255, 165, 0);
    hud_symbols.SetColorMod(255, 165, 0);

    // Symbol money
    symbolX = weidthScreen * 0.75;
    int moneyPosSymbol = 7;

    drawSymbols(money, moneyPosSymbol, symbolX, symbolY);

    symbolX = weidthScreen * 0.45;
    //symbolY = heightScreen * 0.02;
    int timePosSymbol = 2;

    // Symbol time
    drawSymbols(time, timePosSymbol, symbolX, symbolY);

    // Num round
    int anchoNumns = 38;

    symbolX = (weidthScreen * 0.5) - anchoNumns / 2;
    symbolY = heightScreen * 0.02;

    SDL_Rect srcRect = {anchoNumns * (round - 1), 0,anchoNumns, anchoSymbols};
    SDL_Rect destRect = {symbolX, symbolY, 38, 40};

    sdlRenderer->Copy(roundNums, srcRect, destRect);


    // Dibuja mira sin zoom (porque está en coords de pantalla)
    sdlRenderer->Copy(mira, SDL2pp::NullOpt, mouse);
}

void HudSprite::drawSymbols(int& num, int& posSymbol, int& SymbolX, int& SymbolY){
    Texture& hud_symbols = texturas.getTexture("symbols"); 
    Texture& nums = texturas.getTexture("nums"); 

    int anchoNum = 40;
    int anchoNumPantalla = 35;
    int alturaSymbols = 50;
    int anchoSymbols = 64;
    int contador = 0;
    int posx;
    int posy;

    // Symbol health
    SDL_Rect srcRect = {(anchoSymbols + 2) * posSymbol, 0, anchoSymbols, anchoSymbols};
    SDL_Rect destRect = {SymbolX, SymbolY, alturaSymbols, alturaSymbols};

    sdlRenderer->Copy(hud_symbols, srcRect, destRect);

    //Agregamos separacion entre el symbolo y numeros
    SymbolX += 7;

    std::string numStr = std::to_string(num);
    for (char c : numStr){
        int digito = c - '0';

        posx = 49 * digito;
        posy = 0;
        srcRect = {posx, posy, anchoNum, 66};

        posx = SymbolX + (contador + 1) * 48;
        destRect = {posx, SymbolY, anchoNum, alturaSymbols};
        contador++;
        sdlRenderer->Copy(nums, srcRect, destRect);
    }
}