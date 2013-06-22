from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from PySide.QtGui import QFileDialog


class FileDialog(QFileDialog):

    def __init__(
            self, parent=None, message="", directory="", file="",
            pattern="*.*", multiple=False, dir_mode=False):
        QFileDialog.__init__(self, parent, message)
        if directory:
            self.setDirectory(directory)
        if multiple:
            self.setFileMode(QFileDialog.ExistingFiles)

    def get_path(self):
        return self.get_paths()[0]

    def get_paths(self):
        return self.selectedFiles()

    def show_modal(self):
        result = self.exec_()
        return result

    def show(self):
        return self.show_modal()
