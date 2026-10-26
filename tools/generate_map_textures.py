#!/usr/bin/env python3
"""Arma las texturas de cada terreno a partir de los tilesets originales del CS2D.

Los tilesets estan en client/data/maps/tilesets/ (tiles de 32x32 px). Por cada terreno escribe en
client/data/maps/<tema>/:
  floor.png: mosaico de 16x16 tiles de piso elegidos al azar (con semilla fija) entre los del tema.
             El juego y el editor lo repiten por todo el mapa: un tile de 32 px ocupa 4x4 tiles
             del mapa, lo mismo que un jugador.
  wall.png:  el tile de 32x32 con el que se dibujan las estructuras solidas (tambien en mosaico).

Uso: python3 tools/generate_map_textures.py   (requiere Pillow)
"""
import random
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
MAPS = ROOT / "client" / "data" / "maps"
TILESETS = MAPS / "tilesets"
TILE = 32
FLOOR_TILES = 16  # el piso generado mide 16x16 tiles (512x512 px), para que se note poco la repeticion

# Por tema: tileset, tiles de piso (fila, columna) con su peso, y el tile de pared
THEMES = {
    "desert": {
        "tileset": "dust.bmp",
        "floor": {(1, c): 3 for c in range(8)} | {(2, 0): 1},
        "wall": (2, 4),  # caja de madera
    },
    "aztec_village": {
        "tileset": "aztec.bmp",
        "floor": {(1, c): 3 for c in (0, 1, 2, 3, 7, 8)} | {(2, 5): 1, (2, 7): 1},
        "wall": (0, 2),  # bloques de piedra
    },
    "training": {
        "tileset": "cs2dnorm.bmp",
        "floor": {(3, 15): 10, (2, 9): 1, (2, 10): 1},
        "wall": (7, 1),  # caja con marco de metal
    },
}


def tile(tileset, row, col):
    return tileset.crop((col * TILE, row * TILE, (col + 1) * TILE, (row + 1) * TILE))


def build(theme, spec):
    tileset = Image.open(TILESETS / spec["tileset"]).convert("RGB")
    rng = random.Random(theme)
    tiles = list(spec["floor"])
    weights = [spec["floor"][t] for t in tiles]
    floor = Image.new("RGB", (FLOOR_TILES * TILE, FLOOR_TILES * TILE))
    for y in range(FLOOR_TILES):
        for x in range(FLOOR_TILES):
            row, col = rng.choices(tiles, weights)[0]
            floor.paste(tile(tileset, row, col), (x * TILE, y * TILE))
    out = MAPS / theme
    out.mkdir(exist_ok=True)
    floor.save(out / "floor.png")
    tile(tileset, *spec["wall"]).save(out / "wall.png")


if __name__ == "__main__":
    for theme, spec in THEMES.items():
        build(theme, spec)
    print("Texturas generadas en", MAPS)
