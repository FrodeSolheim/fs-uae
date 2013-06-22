import Image


def fix(name, w, h):
    with open(name + ".raw", "rb") as f:
        data = f.read()
    im = Image.fromstring("RGBA", (w, h), data)
    im.save(name + ".png")

fix("title_font", 1024, 132)
fix("menu_font", 1024, 132)

