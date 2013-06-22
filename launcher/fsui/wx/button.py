from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .System import System
from .Widget import Widget


class Button(wx.Button, Widget):

    def __init__(self, parent, title=""):
        if System.macosx:
            # spaces at the end seems to affect the left padding
            # and Mac OS also
            title += "    "
        else:
            title = "  " + title + "  "

        wx.Button.__init__(self, parent.get_container(), -1, title)
        self.Bind(wx.EVT_BUTTON, self.__button_event)
        if not System.macosx:
            self.set_min_height(26)

    def on_activate(self):
        pass

    def __button_event(self, event):
        self.on_activate()
