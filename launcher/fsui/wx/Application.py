from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from fsbc.Application import Application as BaseApplication

wx_app = [None]


def require_wx_app():
    if wx_app[0] is None:
        wx_app[0] = wx.PySimpleApp()
    return wx_app[0]


class Application(BaseApplication):

    def __init__(self, name):
        BaseApplication.__init__(self, name)
        #BaseApplication.set_instance(self)
        require_wx_app()
        self.on_create()

    def on_create(self):
        pass

    def __idle_event(self, event):
        self.on_idle()

    def on_idle(self):
        pass

    def run(self):
        self.main_loop()

    def main_loop(self):
        wx_app[0].MainLoop()

    def run_in_main(self, function, *args, **kwargs):
        wx.CallAfter(function, *args, **kwargs)
