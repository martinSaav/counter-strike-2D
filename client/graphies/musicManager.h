#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <map>
#include <string>

#include <SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "SDL_mixer.h"

enum tipoMusic {
    DISPARO_AK47,
    DISPARO_PISTOL,
    DISPARO_AWP,
    DISPARO_M3,
    KNIFE,
    CAMINANDO,
    STEP,
    TIMER,
    AMBIENTE,
    EXPLOIT,
    PISTOLRELOAD,
    RIFLERELOAD,
    BUY,
    ITEMEQUIP,
    BOMBHASBEENDEFUSED,
    BOMBHASBEENPLANTED,
    COUNTERTERRORISTWIN,
    TERRORISTWIN,
    PLANTINGBOMB,
    NONE
};

class MusicManager {

private:
    Mix_Music* musicaActual = nullptr;

    std::map<tipoMusic, Mix_Music*> ficheroDeMusica;
    std::map<tipoMusic, Mix_Chunk*> ficheroDeChunks;
    std::map<tipoMusic, std::string> ficheroDeUbicaciones;

public:
    explicit MusicManager(std::string& assetsPath);

    ~MusicManager();

    void loadMusic(tipoMusic music, int cantVeces);

    void setVolume(int volume);

    void stopMusic();

    int loadSong(tipoMusic music, int loops = 0, int volume = MIX_MAX_VOLUME / 2);

    void stopAllSongs();

    void stopSongs(int canal);
};
#endif
