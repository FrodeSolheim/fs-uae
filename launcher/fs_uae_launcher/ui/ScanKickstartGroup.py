from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import traceback
from fsgs import fsgs
import fsui as fsui
from fsgs.amiga.Amiga import Amiga
from ..Database import Database
from ..Config import Config
from ..Signal import Signal
from ..I18N import _, ngettext

class KickstartStatusGroup(fsui.Group):

    def __init__(self, parent, title, model):
        self.model = model
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        self.ok_image = fsui.Image("fs_uae_launcher:res/ok_emblem.png")
        self.na_image = fsui.Image("fs_uae_launcher:res/na_emblem.png")

        self.icon = fsui.ImageView(self, self.na_image)
        self.layout.add(self.icon)

        self.layout.add_spacer(10)
        self.label = fsui.Label(self, title)
        self.layout.add(self.label)
        self.update()
        Signal.add_listener("scan_done", self)

    def on_destroy(self):
        Signal.remove_listener("scan_done", self)

    def on_scan_done_signal(self):
        self.update()

    def update(self):
        amiga = Amiga.get_model_config(self.model)
        for sha1 in amiga["kickstarts"]:
            if fsgs.file.find_by_sha1(sha1):
                self.icon.set_image(self.ok_image)
                return
        self.icon.set_image(self.na_image)

class ScanKickstartGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)

        self.layout = fsui.VerticalLayout()
        label = fsui.HeadingLabel(self, _("Available Kickstart Versions"))
        self.layout.add(label, margin=10)

        icon_layout = fsui.HorizontalLayout()
        self.layout.add(icon_layout, fill=True)

        icon_layout.add_spacer(20)
        image = fsui.Image("fs_uae_launcher:res/kickstart.png")
        self.image_view = fsui.ImageView(self, image)
        icon_layout.add(self.image_view, valign=0.0, margin=10)

        vert_layout = fsui.VerticalLayout()
        icon_layout.add(vert_layout, fill=True, expand=True)

        vert_layout.add_spacer(0)

        label = fsui.Label(self, _("You should have kickstart files for "
                "each Amiga model you want to use:"))
        vert_layout.add(label, margin=10)

        hori_layout = fsui.HorizontalLayout()
        vert_layout.add(hori_layout, fill=True)

        self.kickstart_groups = []

        colu_layout = fsui.VerticalLayout()
        hori_layout.add(colu_layout, expand=True, fill=True, margin=10)

        self.add_kickstart_group(colu_layout, "Amiga 1000", "A1000")
        colu_layout.add_spacer(10)
        self.add_kickstart_group(colu_layout, "Amiga 500", "A500")
        colu_layout.add_spacer(10)
        self.add_kickstart_group(colu_layout, "Amiga 500+", "A500+")

        colu_layout = fsui.VerticalLayout()
        hori_layout.add(colu_layout, expand=True, fill=True, margin=10)

        self.add_kickstart_group(colu_layout, "Amiga 600", "A600")
        colu_layout.add_spacer(10)
        self.add_kickstart_group(colu_layout, "Amiga 1200", "A1200")
        colu_layout.add_spacer(10)
        self.add_kickstart_group(colu_layout, "Amiga 3000", "A3000")

        colu_layout = fsui.VerticalLayout()
        hori_layout.add(colu_layout, expand=True, fill=True, margin=10)

        self.add_kickstart_group(colu_layout, "Amiga 4000", "A4000/040")
        colu_layout.add_spacer(10)
        self.add_kickstart_group(colu_layout, "Amiga CD32", "CD32")
        colu_layout.add_spacer(10)
        self.add_kickstart_group(colu_layout, "Commodore CDTV", "CDTV")

    def add_kickstart_group(self, layout, title, model):
        group = KickstartStatusGroup(self, title, model)
        self.kickstart_groups.append(group)
        layout.add(group, fill=True)
