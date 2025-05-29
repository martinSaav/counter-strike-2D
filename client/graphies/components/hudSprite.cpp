#include "hudSprite.h"

HudSprite::HudSprite(Renderer* sdlRenderer, int& weidth, int& height) : 
    Component(sdlRenderer), weidthScreen(weidth), heightScreen(height){

    texturas.loadTexture("mira", "../client/data/mira.png");
    texturas.loadTexture("symbols", "../client/data/hud/hud_symbols.bmp");
    texturas.loadTexture("nums", "../client/data/hud/hud_nums.bmp");
}

void HudSprite::draw(SDL_Rect& mouse, int health, int money){
    Texture& mira = texturas.getTexture("mira");
    Texture& hud_symbols = texturas.getTexture("symbols"); 
    Texture& nums = texturas.getTexture("nums"); 

    // Dibuja mira sin zoom (porque está en coords de pantalla)
    sdlRenderer->Copy(mira, SDL2pp::NullOpt, mouse);

    // Aplicamos el color verde
    nums.SetColorMod(100, 200, 100);
    hud_symbols.SetColorMod(100, 200, 100);

    int anchoNum = 40;
    int alturaSymbols = 60;
    int anchoSymbols = 64;

    int symbolX = weidthScreen * 0.05;
    int symbolY = heightScreen * 0.92;

    // Symbol health
    SDL_Rect srcRect = {0, 0, anchoSymbols, anchoSymbols};
    SDL_Rect destRect = {symbolX, symbolY, 60, alturaSymbols};

    sdlRenderer->Copy(hud_symbols, srcRect, destRect);

    // Health
    std::string healthStr = std::to_string(health);

    int contador = 0;
    int posx;
    int posy;
    for (char c : healthStr){
        int digito = c - '0';

        posx = 49 * digito;
        posy = 0;
        srcRect = {posx, posy, anchoNum, 66};

        posx = symbolX + (contador + 1) * 48;
        destRect = {posx, symbolY, anchoNum, alturaSymbols};
        contador++;
        sdlRenderer->Copy(nums, srcRect, destRect);
    }

    // Aplicamos el color azul
    nums.SetColorMod(100, 150, 255);
    hud_symbols.SetColorMod(100, 150, 255);

    // Symbol money
    symbolX = weidthScreen * 0.75;
    int moneyPosSymbol = 7;

    srcRect = {(anchoSymbols * moneyPosSymbol) + 2, 0, anchoSymbols, anchoSymbols};
    destRect = {symbolX,symbolY,60,alturaSymbols};

    sdlRenderer->Copy(hud_symbols, srcRect, destRect);

    // Money
    std::string moneyStr = std::to_string(money);

    contador = 0;
    for (char c : moneyStr){
        int digito = c - '0';

        posx = 48.8 * digito;
        posy = 0;
        srcRect = {posx, posy, anchoNum, 66};

        posx = symbolX + (contador + 1) * 48;
        destRect = {posx, symbolY, anchoNum, alturaSymbols};
        contador++;
        sdlRenderer->Copy(nums, srcRect, destRect);
    }
}
