#!/usr/bin/env python3
"""Genera las texturas de cada tema de mapa en client/data/maps/<tema>/.

Cada tema tiene:
  floor.png: el piso, un pixel por tile (tiene que medir lo mismo que el mapa en tiles).
  wall.png:  baldosa de 32x32 con la que se dibujan las estructuras.

Uso: python3 tools/generate_map_textures.py   (requiere Pillow)
"""
import random
from pathlib import Path

from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parent.parent
MAPS = ROOT / "client" / "data" / "maps"


def jitter(color, amount, rng):
    return tuple(max(0, min(255, c + rng.randint(-amount, amount))) for c in color)


def desert():
    # El desierto usa los assets originales del juego
    out = MAPS / "desert"
    out.mkdir(exist_ok=True)
    Image.open(MAPS / "default_aztec.png").save(out / "floor.png")
    Image.open(MAPS / "aztec.bmp").crop((480, 64, 512, 96)).save(out / "wall.png")


def aztec_village(width=140, height=140):
    rng = random.Random(7)
    out = MAPS / "aztec_village"
    out.mkdir(exist_ok=True)
    floor = Image.new("RGBA", (width, height))
    px = floor.load()
    block = 7
    for by in range(0, height, block):
        for bx in range(0, width, block):
            # losas de piedra con algun parche de pasto
            mossy = rng.random() < 0.18
            base = (86, 120, 62) if mossy else jitter((150, 140, 118), 14, rng)
            for y in range(by, min(by + block, height)):
                for x in range(bx, min(bx + block, width)):
                    edge = x == bx or y == by
                    color = (110, 102, 86) if edge and not mossy else jitter(base, 6, rng)
                    px[x, y] = color + (255,)
    floor.save(out / "floor.png")

    wall = Image.new("RGBA", (32, 32), (120, 98, 70, 255))
    draw = ImageDraw.Draw(wall)
    for row, y in enumerate(range(0, 32, 8)):
        offset = 0 if row % 2 == 0 else 8
        draw.line([(0, y), (31, y)], fill=(80, 64, 44, 255))
        for x in range(offset, 32, 16):
            draw.line([(x, y), (x, y + 7)], fill=(80, 64, 44, 255))
    draw.rectangle([0, 0, 31, 31], outline=(60, 48, 32, 255))
    wall.save(out / "wall.png")


def training(width=110, height=110):
    rng = random.Random(3)
    out = MAPS / "training"
    out.mkdir(exist_ok=True)
    floor = Image.new("RGBA", (width, height))
    px = floor.load()
    for y in range(height):
        for x in range(width):
            if x % 10 == 0 or y % 10 == 0:
                color = (150, 160, 170)  # grilla de entrenamiento
            else:
                color = jitter((96, 104, 112), 5, rng)
            px[x, y] = color + (255,)
    floor.save(out / "floor.png")

    crate = Image.new("RGBA", (32, 32), (156, 112, 60, 255))
    draw = ImageDraw.Draw(crate)
    draw.rectangle([0, 0, 31, 31], outline=(92, 62, 30, 255), width=3)
    draw.line([(3, 3), (28, 28)], fill=(110, 76, 38, 255), width=3)
    draw.line([(28, 3), (3, 28)], fill=(110, 76, 38, 255), width=3)
    crate.save(out / "wall.png")


if __name__ == "__main__":
    desert()
    aztec_village()
    training()
    print("Texturas generadas en", MAPS)
