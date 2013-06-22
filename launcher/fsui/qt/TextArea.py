from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QTextEdit
from .Widget import Widget


class TextArea(QTextEdit, Widget):

    def __init__(self, parent, text="", read_only=False,
            font_family=None):
        QTextEdit.__init__(self, text, parent.get_container())
        Widget.__init__(self, parent)
        self.setReadOnly(read_only)
        if font_family:
            print("FIXME: not respecting font_family yet")

    def get_text(self):
        return self.toPlainText()

    def set_text(self, text):
        self.setText(text)

    def append_text(self, text, color=None):
        self.append(text.strip())
