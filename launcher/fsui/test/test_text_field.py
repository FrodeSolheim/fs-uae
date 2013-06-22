from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from .WidgetTestCase import WidgetTestCase


class TestTextField(WidgetTestCase):

    def test_text_field(self):
        self.setup_dialog()
        text_field = fsui.TextField(self.dialog)
        text_field.set_text("Hello World")
        text_field.disable()
        text_field.enable()
        self.run_dialog()

    def test_text_field_with_text(self):
        self.setup_dialog()
        text_field = fsui.TextField(self.dialog, "Hello World")
        self.assertEqual(text_field.get_text(), "Hello World")
        self.run_dialog()
