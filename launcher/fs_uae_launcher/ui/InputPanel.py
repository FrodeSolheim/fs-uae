from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext
from .Skin import Skin

class InputPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)

        self.layout = fsui.VerticalLayout()

        from .config.InputGroup import InputGroup
        self.input_group = InputGroup(self, with_more_options=True)
        self.layout.add(self.input_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)

        self.input_group = InputGroup(self, with_more_options=False,
                parallel_ports=True)
        self.layout.add(self.input_group, fill=True)

        self.input_group = InputGroup(self, with_more_options=False,
                custom_ports=True)
        self.layout.add(self.input_group, fill=True)
