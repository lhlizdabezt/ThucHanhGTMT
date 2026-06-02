from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "assets"
OUT = ASSETS / "gtmt-data-flow.gif"


def font(size, bold=False):
    candidates = [
        "C:/Windows/Fonts/segoeuib.ttf" if bold else "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arialbd.ttf" if bold else "C:/Windows/Fonts/arial.ttf",
    ]
    for path in candidates:
        if Path(path).exists():
            return ImageFont.truetype(path, size=size)
    return ImageFont.load_default()


TITLE = font(34, True)
SUB = font(17)
CARD = font(18, True)
SMALL = font(13)
MONO = font(14, True)


def draw_card(draw, box, title, detail, active, fill):
    x1, y1, x2, y2 = box
    draw.rounded_rectangle((x1 + 4, y1 + 6, x2 + 4, y2 + 6), radius=18, fill=(8, 18, 40))
    draw.rounded_rectangle(box, radius=18, fill=fill, outline=(226, 232, 240), width=3 if active else 1)
    draw.text((x1 + 16, y1 + 17), title, font=CARD, fill=(15, 23, 42))
    draw.text((x1 + 16, y1 + 49), detail, font=SMALL, fill=(51, 65, 85))


def frame(active):
    img = Image.new("RGB", (960, 360), (15, 23, 42))
    draw = ImageDraw.Draw(img)
    draw.rounded_rectangle((24, 22, 936, 338), radius=26, fill=(248, 250, 252))
    draw.text((52, 44), "BLE Sensing and RSSI Lab", font=TITLE, fill=(15, 23, 42))
    draw.text((54, 88), "AHT20 data, Android map evidence, UART/VCOM and SQLite logging.", font=SUB, fill=(71, 85, 105))

    cards = [
        ("AHT20", "sensor sample", (219, 234, 254)),
        ("I2C Bus", "embedded read", (204, 251, 241)),
        ("BLE RSSI", "beacon evidence", (254, 249, 195)),
        ("Android Map", "room position", (254, 226, 226)),
        ("UART VCOM", "host transport", (233, 213, 255)),
        ("SQLite", "Windows log", (220, 252, 231)),
    ]
    positions = [
        (52, 140, 318, 218),
        (342, 140, 608, 218),
        (632, 140, 884, 218),
        (52, 234, 318, 312),
        (342, 234, 608, 312),
        (632, 234, 884, 312),
    ]
    for index, (name, detail, color) in enumerate(cards):
        draw_card(draw, positions[index], name, detail, index == active, color)

    draw.rounded_rectangle((662, 42, 896, 72), radius=12, fill=(15, 23, 42))
    snippets = [
        "temp=25.4C hum=63.1",
        "I2C read complete",
        "RSSI beacon sample",
        "room position mapped",
        "UART frame received",
        "SQLite insert OK",
    ]
    draw.text((678, 51), snippets[active], font=MONO, fill=(125, 211, 252))
    return img


def main():
    frames = [frame(i % 6) for i in range(30)]
    frames[0].save(OUT, save_all=True, append_images=frames[1:], duration=170, loop=0, optimize=True)
    print(f"wrote {OUT} ({OUT.stat().st_size} bytes)")


if __name__ == "__main__":
    main()
