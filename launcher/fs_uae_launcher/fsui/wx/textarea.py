from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from .common import update_class
from .System import System

class TextArea(wx.TextCtrl):
    def __init__(self, parent, text="", read_only=False, horizontal_scroll=False,
            font_family=""):
        style = wx.TE_MULTILINE | wx.TE_RICH
        if read_only:
            style = style | wx.TE_READONLY
        if horizontal_scroll:
            style = style | wx.HSCROLL
        wx.TextCtrl.__init__(self, parent.get_container(), -1, text,
                wx.DefaultPosition, wx.DefaultSize, style)
        if font_family == "monospace":
            font = self.GetFont()
            if System.linux:
                font.SetFaceName("Monospace")
                font.SetPointSize(font.GetPointSize() - 1)
            else:
                font.SetFaceName("Courier")
            #font.SetFamily(wx.FONTFAMILY_MODERN)
            self.SetFont(font)

    #def get_min_width(self):
    #    return self.GetBestSize()[0]

    #def get_min_height(self):
    #    return self.GetBestSize()[1]

    def set_position(self, position):
        self.SetPosition(position)

    def set_size(self, size):
        self.SetSize(size)

    def get_text(self):
        return self.GetValue()

    def set_text(self, text):
        self.SetValue(text)

    def append_text(self, text, color=None):
        if color is not None:
            self.SetDefaultStyle(wx.TextAttr(wx.Color(*color)))
        else:
            self.SetDefaultStyle(wx.TextAttr())
        self.AppendText(text)
        if System.windows:
            # hack to work around a problem on Windows where the
            # content scroll one page when adding a single line
            # when the window would need to scroll a line.
            self.ScrollLines(-1)

update_class(TextArea)
