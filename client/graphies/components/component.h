#ifndef COMPONENT_H
#define COMPONENT_H

#include "textureStore.h"

class Component{

    protected:
    TextureStore texturas;
    Renderer* sdlRenderer;

    public:
    // Crea el junto con sus medidas
    Component(Renderer* sdlRenderer) : texturas(sdlRenderer), sdlRenderer(sdlRenderer){};

    // Dibuja el componente
    //virtual void draw();
};
#endif
