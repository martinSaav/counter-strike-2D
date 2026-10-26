#include "musicManager.h"

#include <algorithm>
#include <cstdlib>
#include <utility>
#include <vector>

MusicManager::MusicManager(const std::string& assetsPath) {
    Mix_Init(MIX_INIT_MP3);  // Iniciar SDL_mixer para manejar formato mp3
    // Abrimos un dispositivo
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2,
                  2048);  //(frecuencia, formato, número de canales, tam de buffer)

    // Musica de fondo
    const std::string ambienceGameLocation = assetsPath + "/sounds/ambienceGame.mp3";
    musicFiles[MusicType::AMBIENCE] = Mix_LoadMUS(ambienceGameLocation.c_str());

    // Sonidos cortos. Algunos tienen varias variantes (muerte, dano, pasos) y se elige una al
    // azar cada vez, como en el CS2D original.
    const std::vector<std::pair<MusicType, std::vector<std::string>>> shortSounds = {
            {MusicType::SHOT_PISTOL, {"glock18.wav"}},
            {MusicType::SHOT_AK47, {"ak47.wav"}},
            {MusicType::SHOT_AWP, {"awp.wav"}},
            {MusicType::SHOT_M3, {"m3.wav"}},
            {MusicType::KNIFE, {"knife_slash.wav"}},
            {MusicType::EXPLOIT, {"c4_explode.wav"}},
            {MusicType::PISTOLRELOAD, {"reload.wav"}},
            {MusicType::RIFLERELOAD, {"reload.wav"}},
            {MusicType::BUY, {"buy.mp3"}},
            {MusicType::ITEMEQUIP, {"itemEquip.mp3"}},
            {MusicType::BOMBHASBEENDEFUSED, {"BombHasBeenDefusedCounterTerroristsWin.mp3"}},
            {MusicType::BOMBHASBEENPLANTED, {"BombHasBeenPlanted.mp3"}},
            {MusicType::COUNTERTERRORISTWIN, {"CounterTerroristWin.mp3"}},
            {MusicType::TERRORISTWIN, {"TerroristsWin.mp3"}},
            {MusicType::PLANTINGBOMB, {"PlantingC4Bomb.mp3"}},
            {MusicType::DEATH, {"die1.wav", "die2.wav", "die3.wav"}},
            {MusicType::HIT, {"hit1.wav", "hit2.wav", "hit3.wav"}},
            {MusicType::STEP, {"pl_step1.wav", "pl_step2.wav", "pl_step3.wav", "pl_step4.wav"}},
            {MusicType::DEFUSING, {"c4_disarm.wav"}},
            {MusicType::C4_BEEP, {"c4.wav"}},
            {MusicType::EMPTY_CLICK, {"click.wav"}},
            {MusicType::PICKUP, {"pickup.wav"}},
            {MusicType::DROP, {"drop.wav"}}};

    for (const auto& [type, files]: shortSounds) {
        for (const auto& file: files) {
            if (Mix_Chunk* chunk = Mix_LoadWAV((assetsPath + "/sounds/" + file).c_str())) {
                chunkFiles[type].push_back(chunk);
            }
        }
    }
}

MusicManager::~MusicManager() {

    for (auto& pair: musicFiles) {
        if (pair.second)
            Mix_FreeMusic(pair.second);
    }
    for (auto& [type, chunks]: chunkFiles) {
        for (Mix_Chunk* chunk: chunks) {
            Mix_FreeChunk(chunk);
        }
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
    if (it == chunkFiles.end() || it->second.empty()) {
        throw std::runtime_error("Sound not found");
    }

    Mix_Chunk* chunk = it->second[std::rand() % it->second.size()];
    int channel = Mix_PlayChannel(-1, chunk, loops);
    Mix_Volume(channel, volume);
    return channel;
}

int MusicManager::volumeForDistance(int baseVolume, float distance, float maxDistance) {
    float factor = 1.0f - distance / maxDistance;
    if (factor <= 0) {
        return 0;
    }
    return int(baseVolume * std::min(factor, 1.0f));
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
