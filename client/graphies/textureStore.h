#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include <map>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "SDL_image.h"

using SDL2pp::Renderer;
using SDL2pp::Texture;

class TextureStore {
private:
    // Almacena las texturas
    std::map<std::string, Texture> textures;
    Renderer* sdlRenderer;

public:
    TextureStore(Renderer* sdlRenderer);

    // Cargamos la textura en la memoria
    void loadTexture(const std::string& name, const std::string& path, bool setColorKey = true);

    // Obtenemos la textura
    Texture& getTexture(const std::string& name);

    // Liberamos la textura
    void clearTextures();
};
#endif
