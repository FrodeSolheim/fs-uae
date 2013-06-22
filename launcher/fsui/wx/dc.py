from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .System import System
from .Color import Color

class Font(object):
    def __init__(self, font):
        self.font = font

    def set_bold(self, bold):
        if bold:
            self.font.SetWeight(wx.FONTWEIGHT_BOLD)
        else:
            self.font.SetWeight(wx.FONTWEIGHT_NORMAL)


class DrawingContext(object):

    def __init__(self, dc):
        self.dc = dc
        self.gc = wx.GraphicsContext_Create(dc)

        if System.macosx:
             self.draws_last_line_point = True
        else:
            self.draws_last_line_point = False
        if System.windows:
            self.line_coordinate_offset_x = 0.0
            self.line_coordinate_offset_y = 0.0
        else:
            self.line_coordinate_offset_x = 0.5
            self.line_coordinate_offset_y = 0.5

    def clear(self, color=None):
        if color is not None:
            self.dc.SetBackground(wx.Brush(color))
        self.dc.Clear()

    def get_font(self):
        return Font(self.dc.GetFont())

    def set_font(self, font):
        return self.dc.SetFont(font.font)

    def draw_text(self, text, x, y):
        self.dc.DrawText(text, x, y)

    def measure_text(self, text):
        return self.dc.GetTextExtent(text)

    #def set_color(self, color):
    #    self.dc.SetPen(wx.Pen(color))
    #    #self.gc.SetPen(self.gc.CreatePen(wx.Pen(color)))

    def get_text_color(self):
        return Color(self.dc.GetTextForeground())

    def set_text_color(self, color):
        self.dc.SetTextForeground(color)

    def draw_line(self, x1, y1, x2, y2, c):
        """
        if self.draws_last_line_point:
            if x1 == x2:  # Line is vertical
                if y2 > y1:
                    y2 -= 1
                else:
                    y1 += 1
            elif y1 == y2:  # Line is horizontal
                if x2 > x1:
                    x2 -= 1
                else:
                    x1 += 1
            else:  # Line is neither horizontal nor vertical
                # This correction does not support angular lines.
                pass
        if self.line_coordinate_offset_x:
            x1 += self.line_coordinate_offset_x
            x2 += self.line_coordinate_offset_x
        if self.line_coordinate_offset_y:
            y1 += self.line_coordinate_offset_y
            y2 += self.line_coordinate_offset_y
        print(x1, y1, x2, y2)
        c = wx.Color(255, 0, 0, 255)
        #x1 += 1.0

        #self.dc.DrawLine(x1, y1, x2, y2)
        #self.gc.DrawLine(x1, y1, x2, y2)
        #self.gc.SetPen(wx.NullGraphicsPen)
        #self.gc.SetBrush(self.gc.CreateBrush(wx.Brush(c)))
        #self.gc.DrawRectangle(x1, y1, x2, y2)
        #self.gc.SetPen(wx.NullGraphicsPen)

        self.gc.SetPen(self.gc.CreatePen(wx.Pen(c)))
        self.gc.StrokeLine(x1, y1, x2, y2)
        """

        if x1 == x2:  # Line is vertical
            if y1 > y2:
                y1, y2 = y2, y1
            x2 += 1
        elif y1 == y2:  # Line is horizontal
            if x1 > x2:
                x1, x2 = x2, x1
            y2 += 1
        else:
            # Line is neither horizontal nor vertical
            # This function does not support angular lines.
            return
        self.gc.SetPen(wx.NullGraphicsPen)
        self.gc.SetBrush(self.gc.CreateBrush(wx.Brush(c)))
        self.gc.DrawRectangle(x1, y1, x2 - x1, y2 - y1)

    def draw_image(self, image, x, y, w=None, h=None):
        if w is None and h is None:
            self.dc.DrawBitmap(image.bitmap, x, y, True)
        else:
            self.gc.DrawBitmap(image.bitmap, x, y, w, h)

    def draw_vertical_gradient(self, x, y, w, h, c1, c2):
        #self.dc.GradientFillLinear(wx.Rect(x, y, w, h), c1,
        #        c2, wx.SOUTH)
        #self.gc.SetPen(self.gc.CreatePen(wx.Pen(color)))
        #print(c1, c2)
        brush = self.gc.CreateLinearGradientBrush(x, y, x, y + h, c1, c2)
        self.gc.SetPen(wx.NullGraphicsPen)
        self.gc.SetBrush(brush)
        self.gc.DrawRectangle(x, y, w, h)

    def draw_rectangle(self, x, y, w, h, c):
        brush = self.gc.CreateBrush(wx.Brush(c))
        self.gc.SetPen(wx.NullGraphicsPen)
        self.gc.SetBrush(brush)
        self.gc.DrawRectangle(x, y, w, h)
