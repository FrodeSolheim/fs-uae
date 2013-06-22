from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QPushButton
from .System import System
from .Widget import Widget


class Button(Widget):

    def __init__(self, parent, label):
        label = "  " + label + "  "
        self._widget = QPushButton(label, parent.get_container())
        Widget.__init__(self, parent)
        self._widget.clicked.connect(self.__clicked)
        if not System.macosx:
            self.set_min_height(28)

    def __clicked(self):
        self.on_activate()

    def on_activate(self):
        pass
