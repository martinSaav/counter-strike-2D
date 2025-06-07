#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include <map>
#include <string>
#include "SDL_image.h"
#include <SDL2pp/SDL2pp.hh>

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
    void loadTexture(const std::string& name, const std::string& path);

    // Obtenemos la textura
    Texture& getTexture(const std::string& name);

    // Liberamos la textura
    void clearTextures();
};
#endif
