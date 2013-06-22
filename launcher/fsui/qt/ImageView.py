from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QLabel
from .Widget import Widget


class ImageView(Widget):

    def __init__(self, parent, image):
        self._widget = QLabel(parent.get_container())
        Widget.__init__(self, parent)
        self._widget.setPixmap(image.qpixmap)

    def set_image(self, image):
        self._widget.setPixmap(image.qpixmap)
