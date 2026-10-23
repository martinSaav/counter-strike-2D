#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "../../common/dto/player_info.h"

#include "configuration.h"
#include "musicManager.h"
#include "textureStore.h"

class Component {

protected:
    TextureStore textures;
    Renderer* sdlRenderer;
    Configuration& configuration;
    MusicManager sounds;

    void drawHud(SDL_Rect srcRect, SDL_Rect destRect, std::string& textureName) {
        Texture& texture = textures.getTexture(textureName);
        sdlRenderer->Copy(texture, srcRect, destRect);
    }

    void drawHud2(SDL_Rect destRect, std::string& textureName, double angle = 0) {
        Texture& texture = textures.getTexture(textureName);
        sdlRenderer->Copy(texture, SDL2pp::NullOpt, destRect, angle);
    }

public:
    // Crea el junto con sus medidas
    Component(Renderer* sdlRenderer, Configuration& configuration, std::string& assetsPath):
            textures(sdlRenderer),
            sdlRenderer(sdlRenderer),
            configuration(configuration),
            sounds(assetsPath) {}
};
#endif
