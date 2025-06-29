#include "musicManager.h"

#include <utility>
#include <vector>

MusicManager::MusicManager(const std::string& assetsPath) {
    Mix_Init(MIX_INIT_MP3);  // Iniciar SDL_mixer para manejar formato mp3
    // Abrimos un dispositivo
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2,
                  2048);  //(frecuencia, formato, número de canales, tam de buffer)

    // En el constructor:
    std::string ubicacionPistol = assetsPath + "/sounds/pistolShot.wav";
    std::string ubicacionAk47 = assetsPath + "/sounds/ak47.wav";
    std::string ubicacionKnife = assetsPath + "/sounds/knife.wav";
    // std::string ubicacionStep = assetsPath + "/sounds/step.wav";
    std::string ubicacionTimer = assetsPath + "/sounds/timer.wav";
    std::string ubicacionAmbienceGame = assetsPath + "/sounds/ambienceGame.mp3";
    std::string ubicacionExploit = assetsPath + "/sounds/explosion.mp3";
    std::string ubicacionReload = assetsPath + "/sounds/pistolReload.mp3";
    std::string ubicacionReload2 = assetsPath + "/sounds/rifleReload.mp3";
    std::string ubicacionBuy = assetsPath + "/sounds/buy.mp3";
    std::string ubicacionItemEquip = assetsPath + "/sounds/itemEquip.mp3";
    std::string ubicacionAwp = assetsPath + "/sounds/awp.wav";
    std::string ubicacionM3 = assetsPath + "/sounds/m3.wav";

    std::string ubicacionBombHasBeenDefused =
            assetsPath + "/sounds/BombHasBeenDefusedCounterTerroristsWin.mp3";
    std::string ubicacionBombHasBeenPlanted = assetsPath + "/sounds/BombHasBeenPlanted.mp3";
    std::string ubicacionCounterTerroristWin = assetsPath + "/sounds/CounterTerroristWin.mp3";
    std::string ubicacionTerroristWin = assetsPath + "/sounds/TerroristsWin.mp3";
    std::string ubicacionPlantingBomb = assetsPath + "/sounds/PlantingC4Bomb.mp3";

    ficheroDeUbicaciones[tipoMusic::DISPARO_PISTOL] = ubicacionPistol;
    ficheroDeUbicaciones[tipoMusic::DISPARO_AK47] = ubicacionAk47;
    ficheroDeUbicaciones[tipoMusic::KNIFE] = ubicacionKnife;
    ficheroDeUbicaciones[tipoMusic::TIMER] = ubicacionTimer;
    ficheroDeUbicaciones[tipoMusic::EXPLOIT] = ubicacionExploit;
    ficheroDeUbicaciones[tipoMusic::PISTOLRELOAD] = ubicacionReload;
    ficheroDeUbicaciones[tipoMusic::RIFLERELOAD] = ubicacionReload2;
    ficheroDeUbicaciones[tipoMusic::BUY] = ubicacionBuy;
    ficheroDeUbicaciones[tipoMusic::ITEMEQUIP] = ubicacionItemEquip;
    ficheroDeUbicaciones[tipoMusic::DISPARO_AWP] = ubicacionAwp;
    ficheroDeUbicaciones[tipoMusic::DISPARO_M3] = ubicacionM3;


    ficheroDeUbicaciones[tipoMusic::BOMBHASBEENDEFUSED] = ubicacionBombHasBeenDefused;
    ficheroDeUbicaciones[tipoMusic::BOMBHASBEENPLANTED] = ubicacionBombHasBeenPlanted;
    ficheroDeUbicaciones[tipoMusic::COUNTERTERRORISTWIN] = ubicacionCounterTerroristWin;
    ficheroDeUbicaciones[tipoMusic::TERRORISTWIN] = ubicacionTerroristWin;
    ficheroDeUbicaciones[tipoMusic::PLANTINGBOMB] = ubicacionPlantingBomb;

    Mix_Music* musica;
    musica = Mix_LoadMUS(ubicacionAmbienceGame.c_str());
    ficheroDeMusica[tipoMusic::AMBIENTE] = musica;
    ficheroDeUbicaciones[tipoMusic::AMBIENTE] = ubicacionAmbienceGame;

    // Sonidos cortos
    std::vector<tipoMusic> sonidos = {tipoMusic::DISPARO_PISTOL,
                                      tipoMusic::DISPARO_AK47,
                                      tipoMusic::DISPARO_AWP,
                                      tipoMusic::DISPARO_M3,
                                      tipoMusic::KNIFE,
                                      tipoMusic::TIMER,
                                      tipoMusic::EXPLOIT,
                                      tipoMusic::PISTOLRELOAD,
                                      tipoMusic::RIFLERELOAD,
                                      tipoMusic::BUY,
                                      tipoMusic::ITEMEQUIP,
                                      tipoMusic::BOMBHASBEENDEFUSED,
                                      tipoMusic::BOMBHASBEENPLANTED,
                                      tipoMusic::COUNTERTERRORISTWIN,
                                      tipoMusic::TERRORISTWIN,
                                      tipoMusic::PLANTINGBOMB};

    for (auto tipo: sonidos) {
        ficheroDeChunks[tipo] = Mix_LoadWAV(ficheroDeUbicaciones[tipo].c_str());
    }
}

MusicManager::~MusicManager() {

    for (auto& par: ficheroDeMusica) {
        if (par.second)
            Mix_FreeMusic(par.second);
    }
    for (auto& par: ficheroDeChunks) {
        if (par.second)
            Mix_FreeChunk(par.second);
    }
    Mix_CloseAudio();
    Mix_Quit();
}


void MusicManager::loadMusic(tipoMusic music, int cantVeces) {

    auto it = ficheroDeMusica.find(music);
    if (it == ficheroDeMusica.end()) {
        throw std::runtime_error("Musica no encontrada");
    }

    Mix_Music* musica = it->second;

    if (!musica) {
        throw std::runtime_error("Error cargando música: " + std::string(Mix_GetError()));
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    Mix_PlayMusic(musica, cantVeces);
    musicaActual = musica;
}

int MusicManager::loadSong(tipoMusic music, int loops, int volume) {

    auto it = ficheroDeChunks.find(music);
    if (it == ficheroDeChunks.end()) {
        throw std::runtime_error("Sonido no encontrado");
    }

    Mix_Chunk* chunk = it->second;
    int canal = Mix_PlayChannel(-1, chunk, loops);
    Mix_Volume(canal, volume);
    return canal;
}

void MusicManager::stopMusic() {

    if (musicaActual) {
        // Mix_HaltMusic();  // Detiene la reproducción, no libera memoria
        Mix_FreeMusic(musicaActual);
        musicaActual = nullptr;
    }
}

void MusicManager::stopAllSongs() { Mix_HaltChannel(-1); }

void MusicManager::stopSongs(int canal) { Mix_HaltChannel(canal); }

void MusicManager::setVolume(int volume) { Mix_VolumeMusic(volume); }
