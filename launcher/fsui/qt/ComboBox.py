from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QComboBox
from .Widget import Widget


class ComboBox(Widget):

    def __init__(self, parent, items=[], read_only=False):
        self._widget = QComboBox(parent.get_container())
        Widget.__init__(self, parent)
        #self.setEditable(not read_only)
        print("FIXME: ComboBox not respecting read_only (because of missing lineEdit then")
        self._widget.setEditable(True)

        self.set_items(items)

        if len(items) > 0:
            self.set_index(0)
        self._widget.currentIndexChanged.connect(self.__current_index_changed)

    def __current_index_changed(self):
        self.on_change()

    def set_items(self, items):
        self._widget.clear()
        for i, item in enumerate(items):
            self._widget.insertItem(i, item)

    def set_item_text(self, item, text):
        self._widget.setItemText(item, text)

    def get_index(self):
        #return self.GetSelection()
        return self._widget.currentIndex()

    def set_index(self, index):
        self._widget.setCurrentIndex(index)

    def get_text(self):
        return self._widget.lineEdit().text()

    def set_text(self, text):
        self._widget.lineEdit().setText(text)

    def on_change(self):
        pass
