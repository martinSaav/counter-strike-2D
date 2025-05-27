#include "textureStore.h"

void TextureStore::loadTexture(const std::string& name, const std::string& path, Renderer* sdlRenderer) {

    auto imagen = IMG_Load(path.c_str());
    Texture texture(*sdlRenderer, Surface(imagen).SetColorKey(true, 0));

    textures.emplace(name, std::move(texture));
}

Texture& TextureStore::getTexture(const std::string& name) {

    auto it = textures.find(name);
    if (it == textures.end()) {
        throw std::runtime_error("Textura '" + name + "' no encontrada");
    }
    return it->second;
}

void TextureStore::clearTextures() {
    //Liberamos la textura
    textures.clear();
}
