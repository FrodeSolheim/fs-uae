from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QListView
from PySide.QtGui import QStandardItemModel, QStandardItem
#from PySide.QtCore import QModelIndex
from .Widget import Widget


class ListView(Widget):

    def __init__(self, parent):
        self._widget = QListView(parent.get_container())
        Widget.__init__(self, parent)
        #self.setSelectionModel()
        self._model = QStandardItemModel(self._widget)
        self._widget.setModel(self._model)
        #self.itemSelectionChanged.connect(self._on_selection_changed)
        selection_model = self._widget.selectionModel()
        print("QListView selectionModel", selection_model)
        selection_model.selectionChanged.connect(self.__selection_changed)

    def __selection_changed(self):
        model_index = self._widget.selectionModel().selectedIndexes()[0]
        self.on_select_item(model_index.row())

    def set_default_icon(self, image):
        pass

    def set_items(self, items):
        self._model.clear()
        for label in items:
            item = QStandardItem(label)
            self._model.appendRow(item)

    def set_index(self, index):
        # print(self._widget.rootIndex)
        # idx = QModelIndex.createIndex(index)
        idx = self._model.index(index, 0)
        self._widget.setCurrentIndex(idx)

    def select_item(self, index):
        self.set_index(index)

    def on_select_item(self, index):
        pass
