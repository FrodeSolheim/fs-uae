from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from .MemoryWidget import MemoryWidget

class MemoryGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        _("Memory")
        heading_label = fsui.HeadingLabel(self, _("Override Installed Memory"))
        self.layout.add(heading_label, margin=10)
        self.layout.add_spacer(0)

        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)

        vert_layout = fsui.VerticalLayout()
        hori_layout.add(vert_layout, fill=True, expand=-1)

        widget = MemoryWidget(self, _("Chip RAM"), "chip_memory",
                [((x + 1) * 512) for x in range(16)])
        vert_layout.add(widget, fill=True, margin=10)

        widget = MemoryWidget(self, _("Slow RAM"), "slow_memory",
                [0, 512, 1024, 1792])
        vert_layout.add(widget, fill=True, margin=10)

        vert_layout = fsui.VerticalLayout()
        hori_layout.add(vert_layout, fill=True, expand=-1)

        widget = MemoryWidget(self, _("Fast RAM"), "fast_memory",
                [(x * 1024) for x in range(9)])
        vert_layout.add(widget, fill=True, margin=10)

        widget = MemoryWidget(self, _("Zorro III RAM"), "zorro_iii_memory",
                [0, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072,
                 262144, 524288, 1048576])
        vert_layout.add(widget, fill=True, margin=10)
