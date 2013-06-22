from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import weakref
from PySide.QtCore import Qt, QSize, QAbstractListModel, QModelIndex 
from PySide.QtGui import QListView
from PySide.QtGui import QStandardItemModel, QStandardItem
#from PySide.QtCore import QModelIndex
from .Widget import Widget


class Model(QAbstractListModel):
    
    def __init__(self, parent):
        QAbstractListModel.__init__(self, parent)
        self.parent = weakref.ref(parent)
        self.count = 0
    
    #def set_item_count(self, count):
    #    self.count = count

    def rowCount(self, parent):
        #print("returning count", self.count, "for parent", parent)
        #return self.count
        return self.parent().get_item_count()

    def data(self, index, role):
        row = index.row()
        #print("data for", index, "role", role)
        if role == Qt.SizeHintRole:
            return QSize(26, 26)
        elif role == Qt.DisplayRole:
            #from PySide.QtCore import QString
            #return QString("test")
            #return "test"
            return self.parent().get_item_text(row)
        #return QVariant()


class VerticalItemView(QListView, Widget):

    def __init__(self, parent):
        QListView.__init__(self, parent.get_container())
        #self.setSelectionModel()
        #self.model = QStandardItemModel(self)
        self.model = Model(self)
        self.setModel(self.model)
        #self.itemSelectionChanged.connect(self.__selection_changed)
        selection_model = self.selectionModel()
        print("VerticalItemView selectionModel = ", selection_model)
        selection_model.selectionChanged.connect(self.__selection_changed)

    def __selection_changed(self):
        model_index = self.selectionModel().selectedIndexes()[0]
        self.on_select_item(model_index.row())

    def get_item_text(self, index):
        return ""

#    def set_item_count(self, count):
#        #self.model.rowCoun
#        self.model.set_item_count(count)
#        #self.update()
#        #self.invalidate()
#        self.dataChanged(self.model.createIndex(0, 0),
#                self.model.createIndex(count, 0))

    def set_default_icon(self, image):
        pass

    #def set_items(self, items):
    #    #print("set_items", items)
    #    self.model.clear()
    #    for label in items:
    #        item = QStandardItem(label)
    #        self.model.appendRow(item)

    def set_index(self, index):
        print(self.rootIndex)
        #idx = QModelIndex.createIndex(index)
        idx = self.model.index(index, 0)
        self.setCurrentIndex(idx)

    def select_item(self, index):
        self.set_index(index)

    def on_select_item(self, index):
        pass

    def update(self):
        #self.model.rowCoun
        count = self.get_item_count()
        #self.model.set_item_count(count)
        #self.update()
        #self.invalidate()
        self.dataChanged(self.model.createIndex(0, 0),
                self.model.createIndex(count, 0))
