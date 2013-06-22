from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ...I18N import _, ngettext
from .OptionUI import OptionUI

class InputSettingsPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()
        self.layout.padding_left = 10
        self.layout.padding_right = 10
        self.layout.padding_top = 10
        self.layout.padding_bottom = 20

        def add_option(name):
            self.layout.add(OptionUI.create_group(self, name), fill=True,
                    margin=10)

        label = fsui.HeadingLabel(self, _("Input Settings"))
        self.layout.add(label, margin=10, margin_bottom=20)
        add_option("automatic_input_grab")
        add_option("initial_input_grab")

        label = fsui.HeadingLabel(self, _("Mouse Settings"))
        self.layout.add(label, margin=10, margin_top=20, margin_bottom=20)
        add_option("mouse_speed")
        add_option("middle_click_ungrab")

        label = fsui.HeadingLabel(self, _("Keyboard Settings"))
        self.layout.add(label, margin=10, margin_top=20, margin_bottom=20)
        add_option("swap_ctrl_keys")
