from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QDialog


class Dialog(QDialog):

    def __init__(self, parent=None, title=""):
        QDialog.__init__(self, parent)
        self.layout = None
        self.setWindowTitle(title)
        
        #self.container = wx.Panel(self)
        #self.container.get_window = self.get_window
        #self.Bind(wx.EVT_SIZE, self.__resize_event)
        #self.Bind(wx.EVT_WINDOW_DESTROY, self.__destroy_event)
        #self.Bind(wx.EVT_CLOSE, self.__close_event)
        self.destroy_listeners = []
        self.close_listeners = []

    def get_parent(self):
        return None

    def closeEvent(self, event):
        print("Dialog.closeEvent")
        for function in self.close_listeners:
            print(function)
            function()
        event.accept()

    def add_destroy_listener(self, function):
        self.destroy_listeners.append(function)

    def add_close_listener(self, function):
        self.close_listeners.append(function)

    def get_window(self):
        return self

    def get_container(self):
        return self

    #def show(self):
    #    self.Show()

    #def close(self):
    #    self.Close()

    def show_modal(self):
        #self.setModal(True)
        #return self.showModal()
        return self.exec_()

    def end_modal(self, value):
        #self.EndModal(value)
        self.done(value)

    def center_on_parent(self):
        #self.CenterOnParent()
        print("FIXME:\n\nDialog.center_on_parent")

    #def destroy(self):
    #    #self.Destroy()
    #    print("FIXME: Dialog.destroy does nothing")

    def set_title(self, title):
        self.setWindowTitle(title)

    def get_size(self):
        return self.GetClientSize()

    def set_size(self, size):
        #self.SetClientSize(size)
        #print("FIXME:\n\nDialog.set_size")
        self.resize(size[0], size[1])

    def on_create(self):
        pass

    def on_close(self):
        pass

    def on_destroy(self):
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

    def showEvent(self, event):
        print("showed..")
        self.on_resize()

    def get_size(self):
        return (self.width(), self.height())

    def resizeEvent(self, event):
        print("resized..")
        self.on_resize()

    def on_resize(self):
        if self.layout:
            self.layout.set_size(self.get_size())
            self.layout.update()
