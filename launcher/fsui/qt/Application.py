from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QApplication
from fsbc.Application import Application as BaseApplication


class Application(BaseApplication):

    def __init__(self, name):
        BaseApplication.__init__(self, name)
        self.qt_app = QApplication([])
        self.on_create()

    def on_create(self):
        pass

    def run(self):
        self.qt_app.exec_()
