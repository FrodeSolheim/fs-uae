from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx
from .common import update_class
from .System import System

class Button(wx.Button):
    def __init__(self, parent, title=""):
        if System.macosx:
            # spaces at the end seems to affect the left padding
            # and Mac OS also
            title = title + u"    "
        else:
            title = u"  " + title + u"  "
        wx.Button.__init__(self, parent.get_container(), -1, title)
        self.Bind(wx.EVT_BUTTON, self.__button_event)
        if not System.macosx:
            self.min_height = 26

    def on_activate(self):
        pass

    def __button_event(self, event):
        self.on_activate()

update_class(Button)
