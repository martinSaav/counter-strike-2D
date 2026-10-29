#include "textureStore.h"

#include <utility>

using SDL2pp::Surface;

TextureStore::TextureStore(Renderer* sdlRenderer): sdlRenderer(sdlRenderer) {}

void TextureStore::loadTexture(const std::string& name, const std::string& path, bool setColorKey) {
    if (textures.contains(name)) {
        return;  // el store es compartido: otro componente ya cargo esta textura
    }

    auto image = IMG_Load(path.c_str());
    auto imageSurface = Surface(image);
    if (setColorKey) {
        imageSurface.SetColorKey(true, 0);
    }
    Texture texture(*sdlRenderer, imageSurface);

    textures.emplace(name, std::move(texture));
}

Texture& TextureStore::getTexture(const std::string& name) {

    auto it = textures.find(name);
    if (it == textures.end()) {
        throw std::runtime_error("Texture '" + name + "' not found");
    }
    return it->second;
}

void TextureStore::clearTextures() {
    // Liberamos la textura
    textures.clear();
}
