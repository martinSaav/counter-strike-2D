#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include "../../common/dto/player_info.h"

#include "configuration.h"
#include "musicManager.h"
#include "textureStore.h"

// Base de las partes que dibujan la pantalla (mapa, jugadores, HUD). Las texturas y los sonidos
// son de Render y se comparten entre todos los componentes: el audio se abre una sola vez y
// cada imagen se carga una sola vez.
class Component {

protected:
    TextureStore& textures;
    Renderer* sdlRenderer;
    Configuration& configuration;
    MusicManager& sounds;

    void drawHud(SDL_Rect srcRect, SDL_Rect destRect, std::string& textureName) {
        Texture& texture = textures.getTexture(textureName);
        sdlRenderer->Copy(texture, srcRect, destRect);
    }

    void drawHud2(SDL_Rect destRect, std::string& textureName, double angle = 0) {
        Texture& texture = textures.getTexture(textureName);
        sdlRenderer->Copy(texture, SDL2pp::NullOpt, destRect, angle);
    }

public:
    Component(Renderer* sdlRenderer, Configuration& configuration, TextureStore& textures,
              MusicManager& sounds):
            textures(textures),
            sdlRenderer(sdlRenderer),
            configuration(configuration),
            sounds(sounds) {}
};
#endif
