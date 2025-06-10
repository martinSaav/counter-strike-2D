#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <SDL.h>
#include "SDL_mixer.h"
#include <map>
#include <string>
#include <SDL2pp/SDL2pp.hh>

enum tipoMusic{
    DISPARO,
    CAMINANDO,
    STEP,
    TIMER
};

class MusicManager{

    private:
    Mix_Music* musicaActual = nullptr;

    std::map <tipoMusic,Mix_Music*> ficheroDeMusica;
    
    public:

    MusicManager();

    ~MusicManager();

    void loadMusic(tipoMusic music, int cantVeces);

    void setVolume(int volume);

    void stopMusic();
};
#endif
