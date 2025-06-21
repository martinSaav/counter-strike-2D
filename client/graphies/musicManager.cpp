#include "musicManager.h"
#include <utility>

MusicManager::MusicManager(){
    Mix_Init(MIX_INIT_MP3); // Iniciar SDL_mixer para manejar formato mp3
    // Abrimos un dispositivo
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //(frecuencia, formato, número de canales, tam de buffer)

    // En el constructor:
    std::string ubicacionPistol = "../client/data/sounds/pistolShot.wav";
    std::string ubicacionAk47 = "../client/data/sounds/akShot.mp3";
    std::string ubicacionKnife = "../client/data/sounds/knife.wav";
    std::string ubicacionStep = "../client/data/sounds/step.wav";
    std::string ubicacionTimer = "../client/data/sounds/timer.wav";
    std::string ubicacionAmbienceGame = "../client/data/sounds/ambienceGame.mp3";
    std::string ubicacionExploit = "../client/data/sounds/explosion.mp3";

    ficheroDeUbicaciones[tipoMusic::DISPARO_PISTOL] = ubicacionPistol;
    ficheroDeUbicaciones[tipoMusic::DISPARO_AK47] = ubicacionAk47;
    ficheroDeUbicaciones[tipoMusic::KNIFE] = ubicacionKnife;
    ficheroDeUbicaciones[tipoMusic::TIMER] = ubicacionTimer;
    ficheroDeUbicaciones[tipoMusic::EXPLOIT] = ubicacionExploit;

    Mix_Music *musica; musica = Mix_LoadMUS(ubicacionAmbienceGame.c_str());
    ficheroDeMusica[tipoMusic::AMBIENTE] = musica;
    ficheroDeUbicaciones[tipoMusic::AMBIENTE] = ubicacionAmbienceGame;

    // Sonidos cortos
    ficheroDeChunks[tipoMusic::DISPARO_PISTOL] = Mix_LoadWAV(ficheroDeUbicaciones[tipoMusic::DISPARO_PISTOL].c_str());
    ficheroDeChunks[tipoMusic::DISPARO_AK47] = Mix_LoadWAV(ficheroDeUbicaciones[tipoMusic::DISPARO_AK47].c_str());
    ficheroDeChunks[tipoMusic::KNIFE] = Mix_LoadWAV(ficheroDeUbicaciones[tipoMusic::KNIFE].c_str());
    ficheroDeChunks[tipoMusic::TIMER] = Mix_LoadWAV(ficheroDeUbicaciones[tipoMusic::TIMER].c_str());
    ficheroDeChunks[tipoMusic::EXPLOIT] = Mix_LoadWAV(ficheroDeUbicaciones[tipoMusic::EXPLOIT].c_str());
}

MusicManager::~MusicManager(){

    for (auto& par : ficheroDeMusica) {
        if (par.second) Mix_FreeMusic(par.second);
    }
    for (auto& par : ficheroDeChunks) {
        if (par.second) Mix_FreeChunk(par.second);
    }
    Mix_CloseAudio();
    Mix_Quit();
}


void MusicManager::loadMusic(tipoMusic music, int cantVeces){

    auto it = ficheroDeMusica.find(music);
    if (it == ficheroDeMusica.end()){
        throw std::runtime_error("Musica no encontrada");
    }

    Mix_Music *musica = it->second;

    if (!musica) {
        throw std::runtime_error("Error cargando música: " + std::string(Mix_GetError()));
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);
    Mix_PlayMusic(musica, cantVeces);
    musicaActual = musica;
}

void MusicManager::loadSong(tipoMusic music, int loops){

    auto it = ficheroDeChunks.find(music);
    if (it == ficheroDeChunks.end()) {
        throw std::runtime_error("Sonido no encontrado");
    }

    Mix_Chunk* chunk = it->second;
    int canal = Mix_PlayChannel(-1, chunk, loops);
    Mix_Volume(canal, MIX_MAX_VOLUME / 2);
}

void MusicManager::stopMusic(){

    if (musicaActual) {
        //Mix_HaltMusic();  // Detiene la reproducción, no libera memoria
        Mix_FreeMusic(musicaActual);
        musicaActual = nullptr;
    }
}

void MusicManager::stopAllSongs(){
    Mix_HaltChannel(-1);
}

void MusicManager::stopSongs(int canal){
    Mix_HaltChannel(canal);
}

void MusicManager::setVolume(int volume){
    Mix_VolumeMusic(volume);
}
