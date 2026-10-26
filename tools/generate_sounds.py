#!/usr/bin/env python3
"""Arma los sonidos derivados de los originales del CS2D que usa el juego.

Uso: python3 tools/generate_sounds.py
Escribe client/data/sounds/reload.wav: sacar el cargador (w_clipout.wav), una pausa corta y
meterlo (w_clipin.wav), en un solo sonido para la recarga. Solo usa la biblioteca estandar.
"""
import wave
from pathlib import Path

SOUNDS = Path(__file__).resolve().parent.parent / "client" / "data" / "sounds"
PAUSE_SECONDS = 0.15


def concat(parts, path):
    frames = []
    params = None
    for part in parts:
        if part is None:  # pausa en silencio
            silence_byte = b"\x80" if params.sampwidth == 1 else b"\x00"
            frames.append(silence_byte * int(params.framerate * PAUSE_SECONDS) *
                          params.sampwidth * params.nchannels)
            continue
        with wave.open(str(part), "rb") as src:
            if params is None:
                params = src.getparams()
            frames.append(src.readframes(src.getnframes()))
    with wave.open(str(path), "wb") as out:
        out.setparams(params)
        out.writeframes(b"".join(frames))


if __name__ == "__main__":
    concat([SOUNDS / "w_clipout.wav", None, SOUNDS / "w_clipin.wav"], SOUNDS / "reload.wav")
    print("Sonidos generados en", SOUNDS)
