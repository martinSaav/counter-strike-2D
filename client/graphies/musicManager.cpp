#include "musicManager.h"
#include <utility>

MusicManager::MusicManager(){
    Mix_Init(MIX_INIT_MP3); // Iniciar SDL_mixer para manejar formato mp3
    // Abrimos un dispositivo
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); //(frecuencia, formato, número de canales, tam de buffer)

    // En el constructor:
    std::string ubicacionPistol = "../client/data/sounds/pistolShot.wav";
    std::string ubicacionStep = "../client/data/sounds/step.wav";
    std::string ubicacionTimer = "../client/data/sounds/timer.wav";

    Mix_Music *musica;
    musica = Mix_LoadMUS(ubicacionPistol.c_str());
    ficheroDeMusica[tipoMusic::DISPARO] = musica;

    musica = Mix_LoadMUS(ubicacionTimer.c_str());
    ficheroDeMusica[tipoMusic::TIMER] = musica;
}

MusicManager::~MusicManager(){
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

    Mix_PlayMusic(musica, cantVeces);
    musicaActual = musica;
    return;
}

void MusicManager::stopMusic(){

    if (musicaActual) {
        Mix_FreeMusic(musicaActual);
        musicaActual = nullptr;
    }
}

void MusicManager::setVolume(int volume){
    Mix_VolumeMusic(volume);
}
