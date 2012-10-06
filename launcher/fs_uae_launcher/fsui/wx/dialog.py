from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx

class Dialog(wx.Dialog):

    def __init__(self, parent=None, title=""):
        #print("setting parent to", parent)
        wx.Dialog.__init__(self, parent, -1, title)
        self.container = wx.Panel(self)
        self.container.get_window = self.get_window
        self.Bind(wx.EVT_SIZE, self.__resize_event)
        self.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        self.Bind(wx.EVT_CLOSE, self.__close_event)
        self.destroy_listeners = []
        self.close_listeners = []

    def add_destroy_listener(self, function):
        self.destroy_listeners.append(function)

    def add_close_listener(self, function):
        self.close_listeners.append(function)

    def get_window(self):
        return self

    def get_container(self):
        return self.container

    def show(self):
        self.Show()

    def close(self):
        self.Close()

    def show_modal(self):
        return self.ShowModal()

    def end_modal(self, value):
        self.EndModal(value)

    def center_on_parent(self):
        self.CenterOnParent()

    def destroy(self):
        self.Destroy()

    def set_title(self, title):
        self.SetTitle(title)

    def get_size(self):
        return self.GetClientSize()

    def set_size(self, size):
        self.SetClientSize(size)

    def on_create(self):
        pass

    def on_resize(self):
        if self.layout:
            #print("calling layout.set_size", self.get_size())
            self.layout.set_size(self.get_size())
            self.layout.update()

    def __resize_event(self, event):
        self.on_resize()
        event.Skip()

    def on_destroy(self):
        pass

    def on_close(self):
        pass

    def __destroy_event(self, event):
        for function in self.destroy_listeners:
            function()
        self.on_destroy()

    def __close_event(self, event):
        print("__close_event")
        for function in self.close_listeners:
            function()
        self.on_close()
        self.Destroy()
