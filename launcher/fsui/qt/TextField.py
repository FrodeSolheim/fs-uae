from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QLineEdit
from .Widget import Widget


class TextField(QLineEdit, Widget):

    def __init__(self, parent, text="", read_only=False):
        QLineEdit.__init__(self, text, parent.get_container())
        Widget.__init__(self, parent)
        self.setReadOnly(read_only)
        self.textChanged.connect(self.__text_changed)

    def get_text(self):
        return self.text()

    def set_text(self, text):
        self.setText(text)

    def on_change(self):
        pass

    def __text_changed(self, text):
        self.on_change()
