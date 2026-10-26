#!/usr/bin/env python3
"""Arma los sonidos derivados de los originales del CS2D que usa el juego.

Uso: python3 tools/generate_sounds.py
Escribe en client/data/sounds/:
  reload.wav: sacar el cargador (w_clipout.wav), una pausa corta y meterlo (w_clipin.wav), en un
              solo sonido para la recarga.
  knife.wav:  knife_slash.wav normalizado (el original es muy bajo y casi no se escuchaba).
Solo usa la biblioteca estandar.
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


def normalize(source, path):
    # Lleva el pico al maximo de la escala (archivos de 8 bits sin signo, como los del CS2D)
    with wave.open(str(source), "rb") as src:
        params = src.getparams()
        frames = src.readframes(src.getnframes())
    assert params.sampwidth == 1
    peak = max(abs(b - 128) for b in frames) or 1
    gain = 127 / peak
    out_frames = bytes(max(0, min(255, round(128 + (b - 128) * gain))) for b in frames)
    with wave.open(str(path), "wb") as out:
        out.setparams(params)
        out.writeframes(out_frames)


if __name__ == "__main__":
    concat([SOUNDS / "w_clipout.wav", None, SOUNDS / "w_clipin.wav"], SOUNDS / "reload.wav")
    normalize(SOUNDS / "knife_slash.wav", SOUNDS / "knife.wav")
    print("Sonidos generados en", SOUNDS)
