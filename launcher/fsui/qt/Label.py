from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QLabel
from .Widget import Widget


class Label(Widget):

    def __init__(self, parent, label):
        self._widget = QLabel(label, parent.get_container())
        Widget.__init__(self, parent)

    def set_text(self, label):
        self._widget.setText(label)
