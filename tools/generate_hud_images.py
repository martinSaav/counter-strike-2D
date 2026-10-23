#!/usr/bin/env python3
"""Genera las imagenes de texto del HUD (ayuda de controles y carteles de plantar/desactivar).

Uso: python3 tools/generate_hud_images.py
Escribe en client/data/hud/. Requiere Pillow y la fuente DejaVu Sans (fonts-dejavu).
"""
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "client" / "data" / "hud"
FONT_DIR = Path("/usr/share/fonts/truetype/dejavu")
ORANGE = (255, 165, 0, 255)
GREEN = (120, 210, 120, 255)
WHITE = (235, 235, 235, 255)


def font(size, bold=False):
    name = "DejaVuSans-Bold.ttf" if bold else "DejaVuSans.ttf"
    return ImageFont.truetype(str(FONT_DIR / name), size)


def help_image():
    sections = [
        ("Movimiento", [("W A S D", "moverse"), ("Mouse", "apuntar"), ("Click izq.", "disparar")]),
        ("Armas", [("1 / 2 / 3", "cuchillo / principal / glock"), ("4", "bomba (terroristas)"),
                   ("R", "recargar"), ("E", "levantar arma del piso"), ("G", "tirar arma o bomba")]),
        ("Compra (al inicio de la ronda)", [("Click en la tienda", "comprar"),
                                           ("I / O / P", "AK-47 / M3 / AWP"),
                                           ("K / L", "balas principal / glock")]),
        ("Bomba", [("4 + click en zona", "plantar (quieto)"),
                   ("F (mantener)", "desactivar (CT, quieto)")]),
        ("Otros", [("H", "mostrar / ocultar esta ayuda"), ("F11", "pantalla completa / ventana"),
                   ("Q", "salir")]),
    ]
    width, height = 620, 720
    img = Image.new("RGBA", (width, height), (15, 25, 15, 215))
    draw = ImageDraw.Draw(img)
    draw.rectangle([0, 0, width - 1, height - 1], outline=ORANGE, width=3)
    draw.text((width // 2, 24), "CONTROLES", font=font(30, True), fill=ORANGE, anchor="mt")
    y = 76
    for title, rows in sections:
        draw.text((30, y), title, font=font(20, True), fill=GREEN)
        y += 30
        for key, desc in rows:
            draw.text((50, y), key, font=font(17, True), fill=WHITE)
            draw.text((270, y), desc, font=font(17), fill=WHITE)
            y += 24
        y += 10
    draw.text((width // 2, height - 18), "Presioná H para cerrar", font=font(15),
              fill=ORANGE, anchor="mb")
    img.save(OUT / "help.png")


def label_image(text, name):
    width, height = 360, 40
    img = Image.new("RGBA", (width, height), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    draw.text((width // 2, height // 2), text, font=font(24, True), fill=ORANGE, anchor="mm",
              stroke_width=2, stroke_fill=(0, 0, 0, 255))
    img.save(OUT / name)


if __name__ == "__main__":
    help_image()
    label_image("PLANTANDO BOMBA", "planting.png")
    label_image("DESACTIVANDO BOMBA", "defusing.png")
    print("Imagenes generadas en", OUT)
