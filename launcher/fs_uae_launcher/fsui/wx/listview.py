from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import wx
from .common import update_class
from .itemview import VerticalItemView


class ListView(VerticalItemView):

    def __init__(self, parent):
        VerticalItemView.__init__(self, parent)

        self.__index = 0
        self.__items = []
        self.__default_icon = None

    def set_default_icon(self, icon):
        self.__default_icon = icon

    def on_select_item(self, index):
        pass

    def on_activate_item(self, index):
        pass

    def get_index(self):
        if len(self.selected_items) == 0:
            return -1
        return self.selected_items[0]

    def set_index(self, index):
        self.select_item(index)

    def get_item_icon(self, index):
        return self.__default_icon

    def get_item_text(self, index):
        return self.__items[index]

    def get_item(self, index):
        return self.__items[index]

    def get_item_count(self):
        return len(self.__items)

    def set_items(self, items):
        self.__items = list(items)
        self.update()

update_class(ListView)
