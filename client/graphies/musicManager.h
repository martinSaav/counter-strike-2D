#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <SDL.h>
#include "SDL_mixer.h"
#include <map>
#include <string>
#include <SDL2pp/SDL2pp.hh>

enum tipoMusic{
    DISPARO_AK47,
    DISPARO_PISTOL,
    KNIFE,
    CAMINANDO,
    STEP,
    TIMER,
    AMBIENTE,
    EXPLOIT,
    NONE
};

class MusicManager{

    private:
    Mix_Music* musicaActual = nullptr;

    std::map <tipoMusic,Mix_Music*> ficheroDeMusica;
    std::map<tipoMusic, Mix_Chunk*> ficheroDeChunks;
    std::map <tipoMusic,std::string> ficheroDeUbicaciones;

    public:

    MusicManager();

    ~MusicManager();

    void loadMusic(tipoMusic music, int cantVeces);

    void setVolume(int volume);

    void stopMusic();

    void loadSong(tipoMusic music, int loops = 0);

    void stopAllSongs();

    void stopSongs(int canal);
};
#endif
