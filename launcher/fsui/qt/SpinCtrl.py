from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QSpinBox
from .Widget import Widget


class SpinCtrl(QSpinBox, Widget):

    def __init__(self, parent, min_value, max_value, initial_value):
        self._widget = QSpinBox(parent.get_container())
        self._widget.setRange(min_value, max_value)
        self._widget.setValue(initial_value)
        self._widget.valueChanged.connect(self.__value_changed)

    def get_value(self):
        return self.value()

    def set_value(self, value):
        self.setValue(value)

    def __value_changed(self, value):
        self.on_change()

    def on_change(self):
        pass
