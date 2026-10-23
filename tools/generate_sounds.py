#!/usr/bin/env python3
"""Genera los sonidos sinteticos del juego que no venian con los assets originales.

Uso: python3 tools/generate_sounds.py
Escribe client/data/sounds/death.wav y step_short.wav (un solo paso recortado de step.wav).
Solo usa la biblioteca estandar.
"""
import math
import random
import struct
import wave
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RATE = 44100


def death_sound(path):
    # Golpe grave que cae de frecuencia, con un poco de ruido, y se apaga en ~0.6 s
    random.seed(11)
    duration = 0.6
    samples = []
    phase = 0.0
    for i in range(int(RATE * duration)):
        t = i / RATE
        freq = 180 * math.exp(-3.5 * t) + 50
        phase += 2 * math.pi * freq / RATE
        envelope = math.exp(-6 * t) * min(1.0, t * 200)
        value = 0.75 * math.sin(phase) + 0.25 * random.uniform(-1, 1) * math.exp(-14 * t)
        samples.append(int(max(-1.0, min(1.0, value * envelope)) * 30000))
    with wave.open(str(path), "wb") as out:
        out.setnchannels(1)
        out.setsampwidth(2)
        out.setframerate(RATE)
        out.writeframes(b"".join(struct.pack("<h", s) for s in samples))


def short_step(source, path, seconds=0.45):
    # step.wav son 23 s de pasos seguidos: nos quedamos con el primero y lo apagamos al final
    with wave.open(str(source), "rb") as src:
        params = src.getparams()
        frames = src.readframes(int(params.framerate * seconds))
    count = len(frames) // 2
    samples = list(struct.unpack("<%dh" % count, frames))
    fade = int(params.framerate * 0.08) * params.nchannels
    for i in range(fade):
        samples[count - fade + i] = int(samples[count - fade + i] * (1 - i / fade))
    with wave.open(str(path), "wb") as out:
        out.setparams(params)
        out.writeframes(struct.pack("<%dh" % count, *samples))


if __name__ == "__main__":
    sounds = ROOT / "client" / "data" / "sounds"
    death_sound(sounds / "death.wav")
    short_step(sounds / "step.wav", sounds / "step_short.wav")
    print("Sonidos generados en", sounds)
