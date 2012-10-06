from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class
from .System import System

class Label(wx.StaticText):

    def __init__(self, parent, title=""):
        wx.StaticText.__init__(self, parent.get_container(), -1, title)

    def set_text(self, text):
        self.SetLabel(text)

class BoldLabel(Label):

    def __init__(self, parent, title=""):
        Label.__init__(self, parent, title)
        font = self.GetFont()
        font.SetWeight(wx.BOLD)
        self.SetFont(font)

class HeadingLabel(Label):

    def __init__(self, parent, title=""):
        Label.__init__(self, parent, title)
        font = self.GetFont()
        if System.windows:
            font.SetPointSize(10)
            font.SetFaceName("Arial")
        font.SetWeight(wx.BOLD)
        self.SetFont(font)

update_class(Label)
