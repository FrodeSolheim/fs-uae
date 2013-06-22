from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QPushButton
from .Widget import Widget


class ImageButton(Widget):

    def __init__(self, parent, image):
        self._widget = QPushButton("", parent.get_container())
        Widget.__init__(self, parent)
        self._widget.setIcon(image.qicon)
        self._widget.clicked.connect(self.__clicked)

    def set_image(self, image):
        self._widget.setIcon(image.qicon)

    def __clicked(self):
        self.on_activate()

    def on_activate(self):
        pass
