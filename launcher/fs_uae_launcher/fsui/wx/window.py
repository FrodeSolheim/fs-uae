from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from ..common.element import Element
from .common import update_class

class Window(wx.Frame):

    def __init__(self, parent=None, title=""):
        wx.Frame.__init__(self, parent, -1, title)
        #Element.init(self)
        if parent:
            wx_parent = parent.container
        else:
            wx_parent = None
        self.container = wx.Panel(self)
        self.container.get_window = self.get_window

        #self._window = wx.Frame(wx_parent, -1, title)
        #self._container = wx.Panel(self._window)
        #self.on_create()

        self.Bind(wx.EVT_SIZE, self.__resize_event)
        # hack to work around window maximizing before show on Windows
        self.container.Bind(wx.EVT_SIZE, self.__resize_event)
        self.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        self.Bind(wx.EVT_CLOSE, self.__close_event)
        self.destroy_listeners = []
        self.close_listeners = []

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.container, 1, wx.EXPAND)
        self.SetSizer(sizer)

    def set_background_color(self, color):
        self.container.SetBackgroundColour(color)

    def maximize(self, maximize=True):
        self.Maximize(maximize)
        #print(self.GetSize())

    def is_maximized(self):
        return self.IsMaximized()

    def center_on_screen(self):
        self.CenterOnScreen()

    def set_icon_from_path(self, path):
        print("set_icon_from_path", path)
        icons = wx.IconBundleFromFile(path, wx.BITMAP_TYPE_ICO)
        self.SetIcons(icons)

    def add_destroy_listener(self, function):
        self.destroy_listeners.append(function)

    def get_window(self):
        return self

    def get_container(self):
        return self.container

    def show(self):
        self.Show()
        self.Layout()

    def hide(self):
        self.Hide()

    def destroy(self):
        self.Destroy()

    def set_title(self, title):
        self.SetTitle(title)

    def get_size(self):
        return self.GetClientSize()

    def get_position(self):
        return self.GetPosition()

    def set_size(self, size):
        self.SetClientSize(size)

    def on_create(self):
        pass

    def on_resize(self):
        if self.layout:
            #print("calling layout.set_size", self.get_size())
            self.layout.set_size(self.get_size())
            #self.layout.update()
            #def set_size_after():
            #    print("set_size_after")
            #    self.layout.set_size(self.get_size())
            #wx.CallLater(1000, set_size_after)

    def __resize_event(self, event):
        self.on_resize()
        event.Skip()

    def on_destroy(self):
        pass

    def __destroy_event(self, event):
        for function in self.destroy_listeners:
            function()
        self.on_destroy()

    def on_destroy(self):
        pass

    def on_close(self):
        pass

    def __close_event(self, event):
        print("__close_event")
        for function in self.close_listeners:
            function()
        self.on_close()
        self.Destroy()


update_class(Window)
