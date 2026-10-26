#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <map>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "SDL_mixer.h"

enum MusicType {
    SHOT_AK47,
    SHOT_PISTOL,
    SHOT_AWP,
    SHOT_M3,
    KNIFE,
    WALKING,
    STEP,
    TIMER,
    AMBIENCE,
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
    DEATH,
    DEFUSING,
    HIT,
    C4_BEEP,
    EMPTY_CLICK,
    PICKUP,
    DROP,
    NONE
};

class MusicManager {

private:
    Mix_Music* currentMusic = nullptr;

    std::map<MusicType, Mix_Music*> musicFiles;
    std::map<MusicType, std::vector<Mix_Chunk*>> chunkFiles;

public:
    explicit MusicManager(const std::string& assetsPath);

    ~MusicManager();

    void loadMusic(MusicType music, int times);

    void setVolume(int volume);

    void stopMusic();

    int loadSong(MusicType music, int loops = 0, int volume = MIX_MAX_VOLUME / 2);

    // Volumen para un sonido que ocurre a cierta distancia (en tiles) del jugador: baja de forma
    // lineal y a partir de maxDistance ya no se escucha.
    static int volumeForDistance(int baseVolume, float distance, float maxDistance = 80.0f);

    void stopAllSongs();

    void stopSongs(int channel);
};
#endif
