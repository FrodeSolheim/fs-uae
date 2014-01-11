from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QColor


class BaseColor(object):

    def mix(self, color, opacity=0.5):
        #print("mix", color)
        """Mixes this color with another color and returns the result.

        Arguments:
        color -- The overlay color to mix in (Color or wx.Colour)
        opacity -- The opacity of the overlay color in the range [0.0, 1.0]

        Returns a reference to self.
        """
        assert opacity >= 0.0 and opacity <= 1.0, "Invalid opacity"
        iopacity = 1 - opacity
        #return wx.Colour(
        self.set_components(
                int(self[0] * iopacity + color[0] * opacity),
                int(self[1] * iopacity + color[1] * opacity),
                int(self[2] * iopacity + color[2] * opacity))
        return self


class Color(QColor, BaseColor):

    def __init__(self, *args):
        QColor.__init__(self)

        if len(args) == 1:
            c = args[0]
            if isinstance(c, QColor):
                self.setRgb(c.red(), c.green(), c.blue(), c.alpha())
            else:
                if len(c) == 3:
                    r, g, b = c
                    self.setRgb(r, g, b)
                else:
                    r, g, b, a = c
                    self.setRgba(r, g, b, a)
        elif len(args) == 3:
            self.setRgb(*args)
        elif len(args) == 4:
            self.setRgb(args[0], args[1], args[2])
            self.setAlpha(args[3])
        else:
            raise TypeError("Color object is not initialized")

    @staticmethod
    def mix_colors (basecolor, overlaycolor, opacity=0.5):
        c = Color(basecolor)
        return c.mix(overlaycolor, opacity)

    def set_components(self, r, g, b, a=255):
        self.setRgb(r, g, b)
        self.setAlpha(a)

    def __getitem__(self, index):
        if index == 0:
            return self.red()
        if index == 1:
            return self.green()
        if index == 2:
            return self.blue()
        raise IndexError("Invalid color component")

    def to_hsv(self):
        return HSVColor.from_rgb(self.red(), self.green(), self.blue())

    def to_hsl(self):
        return HSLColor.from_rgb(self.red(), self.green(), self.blue())

    def lighten(self, amount=0.05):
        c = self.to_hsl().lighten(amount).to_rgb()
        self.set_components(*c)
        return self

    def darken(self, amount=0.05):
        c = self.to_hsl().darken(amount).to_rgb()
        self.set_components(*c)
        return self

#    def mix(self, color, opacity=0.5):
#        """Mixes this color with another color and returns the result.
#
#        Arguments:
#        color -- The overlay color to mix in (Color or wx.Colour)
#        opacity -- The opacity of the overlay color in the range [0.0, 1.0]
#
#        Returns a reference to self.
#        """
#        assert opacity >= 0.0 and opacity <= 1.0, "Invalid opacity"
#        iopacity = 1 - opacity
#        #return wx.Colour(
#        self.Set(
#                int(self.Red() * iopacity + color.Red() * opacity),
#                int(self.Green() * iopacity + color.Green() * opacity),
#                int(self.Blue() * iopacity + color.Blue() * opacity))
#        return self

    def invert(self):
        self.Set(255 - self.Red(), 255 - self.Green(), 255 - self.Blue())
        return self

    def inverted(self):
        return self.copy().invert()

    def copy(self):
        return Color(*list(self))

    def complement(self):
        r, g, b = self.Red(), self.Green(), self.Blue()
        baseval = max(r, max(g, b)) + min(r, min(g, b))
        self.Set(baseval - self.Red(), baseval - self.Green(),
                baseval - self.Blue())
        return self

    def complemented(self):
        return self.copy().complement()

    @property
    def intensity(self):
        return (self.Red() + self.Blue() + self.Green()) // 3

"""
class HSVColor(BaseColor):

    def __init__(self):
        self.h = 0.0
        self.s = 0.0
        self.v = 0.0

    @staticmethod
    def from_rgb(r, g, b):
        r = r / 255
        g = g / 255
        b = b / 255
        c = HSVColor()
        ma = max(r, max(g, b))
        mi = min(r, min(g, b))
        if ma == mi:
            c.h = 0
        elif ma == r:
            c.h = (60 * (g - b)/(ma - mi)) % 360
        elif ma == g:
            c.h = (60 * (b - r)/(ma - mi)) + 120
        else:
            c.h = (60 * (r - g)/(ma - mi)) + 240
        if ma == 0:
            c.s = 0
        else:
            c.s = 1 - mi/ma
        c.v = ma
        return c

    #def ints(self):

    def darken(self, amount=0.05):
        self.v = max(0.0, self.v - amount)
        return self

    def lighten(self, amount=0.05):
        self.v = min(1.0, self.v + amount)
        return self

    def to_rgb(self):
        hi = (self.h // 60) % 6
        f = self.h / 60 - self.h // 60
        p = int(255 * self.v * (1 - self.s))
        q = int(255 * self.v * (1 - f * self.s))
        t = int(255 * self.v * (1 - (1 - f) * self.s))
        v = int(255 * self.v)
        if hi == 0:
            return Color(v, t, p)
        elif hi == 1:
            return Color(q, v, p)
        elif hi == 2:
            return Color(p, v, q)
        elif hi == 3:
            return Color(p, q, v)
        elif hi == 4:
            return Color(t, p, v)
        elif hi == 5:
            return Color(v, p, q)
        raise RuntimeError("Invalid 'hi'-value in to_rgb")

    def __getitem__(self, index):
        if index == 0:
            return int(round(self.h))
        if index == 1:
            return int(round(self.s * 100))
        if index == 2:
            return int(round(self.v * 100))
        raise IndexError("Invalid color component")

    def set_components(self, h, s, v):
        self.h = h
        self.s = s
        self.v = v
"""


class HSLColor(BaseColor):

    def __init__(self):
        self.h = 0.0
        self.s = 0.0
        self.l = 0.0

    @staticmethod
    def from_rgb(r, g, b):
        r = r / 255
        g = g / 255
        b = b / 255
        c = HSLColor()
        ma = max(r, max(g, b))
        mi = min(r, min(g, b))
        if ma == mi:
            c.h = 0
        elif ma == r:
            c.h = (60 * (g - b) / (ma - mi)) % 360
        elif ma == g:
            c.h = (60 * (b - r) / (ma - mi)) + 120
        else:
            c.h = (60 * (r - g) / (ma - mi)) + 240

        c.l = (ma + mi)/2

        if ma == mi:
            c.s = 0
        elif c.l <= 0.5:
            c.s = (ma - mi) / (2 * c.l)
        else:  # c.l > 0.5
            c.s = (ma - mi) / (2 - 2 * c.l)
        return c

    def darken(self, amount=0.05):
        self.l = max(0.0, self.l - amount)
        return self

    def lighten(self, amount=0.05):
        self.l = min(1.0, self.l + amount)
        return self

    def to_rgb(self):

        if self.l < 0.5:
            q = self.l * (1 + self.s)
        else: # c.l >= 0.5
            q = self.l + self.s - (self.l * self.s)
        p = 2 * self.l - q
        hk = self.h / 360

        # t = [tr, tg, tb]
        t = [hk + 1/3, hk, hk - 1/3]

        for i in range(3):
            if t[i] < 0.0:
                t[i] += 1.0
            elif t[1] > 1.0:
                t[i] -= 1.0

        for i in range(3):
            if t[i] < 1/6:
                t[i] = int(round(255 * (p + ((q - p) * 6 * t[i]))))
            elif 1/6 <= t[i] < 1/2:
                t[i] = int(round(255 * q))
            elif 1/2 <= t[i] < 2/3:
                t[i] = int(round(255 * (p + ((q - p) * 6 * (2/3 - t[i])))))
            else:
                t[i] = int(round(255 * p))
        return Color(t)

    def __getitem__(self, index):
        if index == 0:
            return int(round(self.h))
        if index == 1:
            return int(round(self.s * 100))
        if index == 2:
            return int(round(self.l * 100))
        raise IndexError("Invalid color component")

    def set_components(self, h, s, l):
        self.h = h
        self.s = s
        self.l = l
