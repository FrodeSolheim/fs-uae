from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtCore import Qt
from PySide.QtGui import QMainWindow


class Window(QMainWindow):

    def __init__(self, parent=None, title=""):
        QMainWindow.__init__(self, parent)
        self.setWindowTitle(title)
        self.destroyed.connect(self.__destroyed)
        self.setAttribute(Qt.WA_DeleteOnClose, True)

        self.destroy_listeners = []
        self.close_listeners = []

    def get_container(self):
        return self

    def get_parent(self):
        return None

    def add_destroy_listener(self, function):
        self.destroy_listeners.append(function)

    #def add_close_listener(self, function):
    #    self.close_listeners.append(function)

    def closeEvent(self, event):
        print("Window.closeEvent")
        for function in self.close_listeners:
            function()
        event.accept()
        #self.destroy()
         #event.ignore()
 
    def get_window(self):
        return self
 
    def on_destroy(self):
        pass

    def __destroyed(self):
        print("__destroyed")
        for function in self.destroy_listeners:
            function()
        self.on_destroy()

    def showEvent(self, event):
        print("showed..")
        self.on_resize()

    def get_position(self):
        return self.pos().x(), self.pos().y()

    def get_size(self):
        return (self.width(), self.height())

    def set_size(self, size):
        #self.SetClientSize(size)
        #print("FIXME:\n\nDialog.set_size")
        self.resize(size[0], size[1])

    def is_maximized(self):
        #return self.isMaximized()
        print("FIXME: always returning False")
        return False

    def maximize(self):
        self.showMaximized()

    def center_on_parent(self):
        #self.CenterOnParent()
        print("FIXME:\n\nWindow.center_on_parent")

    def center_on_screen(self):
        #self.CenterOnParent()
        print("FIXME:\n\nWindow.center_on_screen")

    def resizeEvent(self, event):
        print("resized..")
        self.on_resize()

    def on_resize(self):
        if self.layout:
            self.layout.set_size(self.get_size())
            self.layout.update()

    def set_background_color(self, color):
        print("FIXME: Window.set_background_color")

    def set_icon_from_path(self, path):
        print("FIXME: Window.set_icon_from_path")
