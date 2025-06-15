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

    public:
    // Crea el junto con sus medidas
    Component(Renderer* sdlRenderer, Configuracion& configuracion) : texturas(sdlRenderer),
        sdlRenderer(sdlRenderer), configuracion(configuracion){};

    // Dibuja el componente
    //virtual void draw();
};
#endif
