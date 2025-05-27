#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include <SDL2pp/SDL2pp.hh>
#include "SDL_image.h"
#include <map>

using namespace SDL2pp;

class TextureStore{
    private:
    //Almacena las texturas
    std::map<std::string, Texture> textures;

    public:

    // Cargamos la textura en la memoria
    void loadTexture(const std::string& name, const std::string& path, Renderer* sdlRenderer);

    //Obtenemos la textura
    Texture& getTexture(const std::string& name);

    //Liberamos la textura
    void clearTextures();
};
#endif
