from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QComboBox
from .Widget import Widget


class Choice(Widget):

    def __init__(self, parent, items=[]):
        self._widget = QComboBox(parent.get_container())
        Widget.__init__(self, parent)
        self.inhibit_change_event = False

        for i, item in enumerate(items):
            self._widget.insertItem(i, item)

        if len(items) > 0:
            self.set_index(0)
        self._widget.currentIndexChanged.connect(self.__current_index_changed)

    def __current_index_changed(self):
        if not self.inhibit_change_event:
            # print("Choice.__current_index_changed")
            return self.on_change()

    def get_index(self):
        return self._widget.currentIndex()

    def set_index(self, index):
        try:
            self.inhibit_change_event = True
            self._widget.setCurrentIndex(index)
        finally:
            self.inhibit_change_event = False

    def on_change(self):
        pass
