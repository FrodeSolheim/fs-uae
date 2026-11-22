class Colour(tuple[int, int, int, int]):
    BLACK = (0x00, 0x00, 0x00, 0xFF)
    GREY_11 = (0x11, 0x11, 0x11, 0xFF)
    GREY_22 = (0x22, 0x22, 0x22, 0xFF)
    GREY_33 = (0x33, 0x33, 0x33, 0xFF)
    GREY_44 = (0x44, 0x44, 0x44, 0xFF)
    GREY_55 = (0x55, 0x55, 0x55, 0xFF)
    GREY_66 = (0x66, 0x66, 0x66, 0xFF)
    GREY_77 = (0x77, 0x77, 0x77, 0xFF)
    GREY_88 = (0x88, 0x88, 0x88, 0xFF)
    GREY_99 = (0x99, 0x99, 0x99, 0xFF)
    GREY_AA = (0xAA, 0xAA, 0xAA, 0xFF)
    GREY_BB = (0xBB, 0xBB, 0xBB, 0xFF)
    GREY_CC = (0xCC, 0xCC, 0xCC, 0xFF)
    GREY_DD = (0xDD, 0xDD, 0xDD, 0xFF)
    GREY_EE = (0xEE, 0xEE, 0xEE, 0xFF)
    RED = (0xFF, 0x00, 0x00, 0xFF)
    WHITE = (0xFF, 0xFF, 0xFF, 0xFF)


TColour = tuple[int, int, int, int]

# Positions, sizes, padding, borders and mouse positions are in design units (fsgui/scale.py).
ExpandParam = bool | float
Padding = tuple[float, float, float, float] | float
PaddingParam = Padding | None
Position = tuple[float, float]
PointerPosition = tuple[float, float]
Size = tuple[float, float]
SizeParam = Size | None

# In pixels: surfaces, the host window, and what a drawing context rounds design units to.
PixelPosition = tuple[int, int]
PixelSize = tuple[int, int]
# x, y, width, height.
PixelRect = tuple[int, int, int, int]


class Borders:
    top: float = 0
    right: float = 0
    bottom: float = 0
    left: float = 0

    def __init__(self, top: float, right: float, bottom: float, left: float):
        self.top = top
        self.right = right
        self.bottom = bottom
        self.left = left
