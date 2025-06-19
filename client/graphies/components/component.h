#ifndef COMPONENT_H
#define COMPONENT_H

#include "textureStore.h"
#include "musicManager.h"
#include "configuracion.h"
#include "../../common/dto/player_info.h"

class Component{

    protected:
    TextureStore texturas;
    MusicManager sounds;
    Renderer* sdlRenderer;
    Configuracion& configuracion;

    void Component::drawHud(SDL_Rect srcRect, SDL_Rect destRect, std::string& textureName){
        Texture& texture = texturas.getTexture(textureName);
        sdlRenderer->Copy(texture, srcRect, destRect);
    }

    void Component::drawHud2(SDL_Rect destRect, std::string& textureName){
        Texture& texture = texturas.getTexture(textureName);
        sdlRenderer->Copy(texture, SDL2pp::NullOpt, destRect);
    }

    public:
    // Crea el junto con sus medidas
    Component(Renderer* sdlRenderer, Configuracion& configuracion) : texturas(sdlRenderer),
        sdlRenderer(sdlRenderer), configuracion(configuracion){};
};
#endif
