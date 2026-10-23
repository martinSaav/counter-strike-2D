#include "musicManager.h"

#include <utility>
#include <vector>

MusicManager::MusicManager(const std::string& assetsPath) {
    Mix_Init(MIX_INIT_MP3);  // Iniciar SDL_mixer para manejar formato mp3
    // Abrimos un dispositivo
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2,
                  2048);  //(frecuencia, formato, número de canales, tam de buffer)

    // En el constructor:
    std::string pistolLocation = assetsPath + "/sounds/pistolShot.wav";
    std::string ak47Location = assetsPath + "/sounds/ak47.wav";
    std::string knifeLocation = assetsPath + "/sounds/knife.wav";
    // std::string ubicacionStep = assetsPath + "/sounds/step.wav";
    std::string timerLocation = assetsPath + "/sounds/timer.wav";
    std::string ambienceGameLocation = assetsPath + "/sounds/ambienceGame.mp3";
    std::string exploitLocation = assetsPath + "/sounds/explosion.mp3";
    std::string reloadLocation = assetsPath + "/sounds/pistolReload.mp3";
    std::string reload2Location = assetsPath + "/sounds/rifleReload.mp3";
    std::string buyLocation = assetsPath + "/sounds/buy.mp3";
    std::string itemEquipLocation = assetsPath + "/sounds/itemEquip.mp3";
    std::string awpLocation = assetsPath + "/sounds/awp.wav";
    std::string m3Location = assetsPath + "/sounds/m3.wav";

    std::string bombHasBeenDefusedLocation =
            assetsPath + "/sounds/BombHasBeenDefusedCounterTerroristsWin.mp3";
    std::string bombHasBeenPlantedLocation = assetsPath + "/sounds/BombHasBeenPlanted.mp3";
    std::string counterTerroristWinLocation = assetsPath + "/sounds/CounterTerroristWin.mp3";
    std::string terroristWinLocation = assetsPath + "/sounds/TerroristsWin.mp3";
    std::string plantingBombLocation = assetsPath + "/sounds/PlantingC4Bomb.mp3";

    locationFiles[MusicType::SHOT_PISTOL] = pistolLocation;
    locationFiles[MusicType::SHOT_AK47] = ak47Location;
    locationFiles[MusicType::KNIFE] = knifeLocation;
    locationFiles[MusicType::TIMER] = timerLocation;
    locationFiles[MusicType::EXPLOIT] = exploitLocation;
    locationFiles[MusicType::PISTOLRELOAD] = reloadLocation;
    locationFiles[MusicType::RIFLERELOAD] = reload2Location;
    locationFiles[MusicType::BUY] = buyLocation;
    locationFiles[MusicType::ITEMEQUIP] = itemEquipLocation;
    locationFiles[MusicType::SHOT_AWP] = awpLocation;
    locationFiles[MusicType::SHOT_M3] = m3Location;


    locationFiles[MusicType::BOMBHASBEENDEFUSED] = bombHasBeenDefusedLocation;
    locationFiles[MusicType::BOMBHASBEENPLANTED] = bombHasBeenPlantedLocation;
    locationFiles[MusicType::COUNTERTERRORISTWIN] = counterTerroristWinLocation;
    locationFiles[MusicType::TERRORISTWIN] = terroristWinLocation;
    locationFiles[MusicType::PLANTINGBOMB] = plantingBombLocation;

    Mix_Music* musicTrack;
    musicTrack = Mix_LoadMUS(ambienceGameLocation.c_str());
    musicFiles[MusicType::AMBIENCE] = musicTrack;
    locationFiles[MusicType::AMBIENCE] = ambienceGameLocation;

    // Sonidos cortos
    std::vector<MusicType> shortSounds = {MusicType::SHOT_PISTOL,
                                          MusicType::SHOT_AK47,
                                          MusicType::SHOT_AWP,
                                          MusicType::SHOT_M3,
                                          MusicType::KNIFE,
                                          MusicType::TIMER,
                                          MusicType::EXPLOIT,
                                          MusicType::PISTOLRELOAD,
                                          MusicType::RIFLERELOAD,
                                          MusicType::BUY,
                                          MusicType::ITEMEQUIP,
                                          MusicType::BOMBHASBEENDEFUSED,
                                          MusicType::BOMBHASBEENPLANTED,
                                          MusicType::COUNTERTERRORISTWIN,
                                          MusicType::TERRORISTWIN,
                                          MusicType::PLANTINGBOMB};

    for (auto type: shortSounds) {
        chunkFiles[type] = Mix_LoadWAV(locationFiles[type].c_str());
    }
}

MusicManager::~MusicManager() {

    for (auto& pair: musicFiles) {
        if (pair.second)
            Mix_FreeMusic(pair.second);
    }
    for (auto& pair: chunkFiles) {
        if (pair.second)
            Mix_FreeChunk(pair.second);
    }
    Mix_CloseAudio();
    Mix_Quit();
}


void MusicManager::loadMusic(MusicType music, int times) {

    auto it = musicFiles.find(music);
    if (it == musicFiles.end()) {
        throw std::runtime_error("Music not found");
    }

    Mix_Music* musicTrack = it->second;

    if (!musicTrack) {
        throw std::runtime_error("Error loading music: " + std::string(Mix_GetError()));
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
    Mix_PlayMusic(musicTrack, times);
    currentMusic = musicTrack;
}

int MusicManager::loadSong(MusicType music, int loops, int volume) {

    auto it = chunkFiles.find(music);
    if (it == chunkFiles.end()) {
        throw std::runtime_error("Sound not found");
    }

    Mix_Chunk* chunk = it->second;
    int channel = Mix_PlayChannel(-1, chunk, loops);
    Mix_Volume(channel, volume);
    return channel;
}

void MusicManager::stopMusic() {

    if (currentMusic) {
        // Mix_HaltMusic();  // Detiene la reproducción, no libera memoria
        Mix_FreeMusic(currentMusic);
        currentMusic = nullptr;
    }
}

void MusicManager::stopAllSongs() { Mix_HaltChannel(-1); }

void MusicManager::stopSongs(int channel) { Mix_HaltChannel(channel); }

void MusicManager::setVolume(int volume) { Mix_VolumeMusic(volume); }
