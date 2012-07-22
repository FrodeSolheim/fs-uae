from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import wx

class Application:

    def __init__(self):
        self.wx_app = wx.PySimpleApp()
        self.on_create()

    def on_create(self):
        pass

    def run(self):
        self.wx_app.MainLoop()
