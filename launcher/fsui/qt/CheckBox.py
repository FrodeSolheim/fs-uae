from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QCheckBox
from .Widget import Widget


class CheckBox(Widget):

    def __init__(self, parent, text=""):
        self._widget = QCheckBox(text, parent.get_container())
        Widget.__init__(self, parent)
        self._widget.stateChanged.connect(self.__state_changed)

    def __state_changed(self):
        self.on_change()

    def is_checked(self):
        return self._widget.isChecked()

    def check(self, checked=True):
        self._widget.setChecked(checked)

    def on_change(self):
        pass
