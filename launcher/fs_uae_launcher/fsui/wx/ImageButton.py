from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class
from .System import System

class ImageButton(wx.BitmapButton):
    def __init__(self, parent, image):
        wx.BitmapButton.__init__(self, parent.get_container(), -1, image.bitmap)
        self.Bind(wx.EVT_BUTTON, self.__button_event)
        if not System.macosx:
            self.min_height = 26

    def on_activate(self):
        pass

    def __button_event(self, event):
        self.on_activate()

update_class(ImageButton)
