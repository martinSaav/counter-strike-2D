#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "../../common/dto/player_info.h"

#include "configuracion.h"
#include "musicManager.h"
#include "textureStore.h"

class Component {

protected:
    TextureStore texturas;
    MusicManager sounds;
    Renderer* sdlRenderer;
    Configuracion& configuracion;

    void drawHud(SDL_Rect srcRect, SDL_Rect destRect, std::string& textureName) {
        Texture& texture = texturas.getTexture(textureName);
        sdlRenderer->Copy(texture, srcRect, destRect);
    }

    void drawHud2(SDL_Rect destRect, std::string& textureName, double angle = 0) {
        Texture& texture = texturas.getTexture(textureName);
        sdlRenderer->Copy(texture, SDL2pp::NullOpt, destRect, angle);
    }

public:
    // Crea el junto con sus medidas
    Component(Renderer* sdlRenderer, Configuracion& configuracion):
            texturas(sdlRenderer), sdlRenderer(sdlRenderer), configuracion(configuracion) {}
};
#endif
