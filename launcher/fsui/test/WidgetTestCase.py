from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import unittest
import fsui as fsui


class WidgetTestCase(unittest.TestCase):

    def __init__(self, methodName="runTest"):
        unittest.TestCase.__init__(self, methodName=methodName)
        self.dialog = None

    def setup_dialog(self):
        self.dialog = fsui.Dialog()

    def run_dialog(self):
        def run_in_dialog():
            self.dialog.end_modal(True)

        fsui.call_after(run_in_dialog)
        self.dialog.show_modal()
        self.dialog.destroy()
