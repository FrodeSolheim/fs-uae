from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import traceback
import fs_uae_launcher.fsui as fsui
from .Amiga import Amiga
from .Database import Database
from .Config import Config
from .Settings import Settings
from .I18N import _, ngettext

class KickstartStatusGroup(fsui.Group):

    def __init__(self, parent, title, model):
        self.model = model
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()

        self.ok_image = fsui.Image("fs_uae_launcher:res/ok_emblem.png")
        self.na_image = fsui.Image("fs_uae_launcher:res/na_emblem.png")

        #self.status = fsui.Label(self, "N/A")
        #self.layout.add(self.status)
        self.icon = fsui.ImageView(self, self.na_image)
        self.layout.add(self.icon)

        self.layout.add_spacer(10)
        self.label = fsui.Label(self, title)
        self.layout.add(self.label)
        self.update()
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_setting(self, key, value):
        if key == "last_scan":
            self.update()

    def update(self):
        database = Database.get_instance()

        amiga = Amiga.get_model_config(self.model)
        print(amiga["kickstarts"])
        for sha1 in amiga["kickstarts"]:
            #print("-----", repr(sha1))
            if database.find_file(sha1=sha1):
                # ok
                #self.status.set_text("OK")
                self.icon.set_image(self.ok_image)
                return
        #self.status.set_text("N/A")
        self.icon.set_image(self.na_image)

class ScanKickstartGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_right = 20
        self.layout.add_spacer(20)

        image = fsui.Image("fs_uae_launcher:res/kickstart.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.layout2 = fsui.VerticalLayout()
        self.layout.add(self.layout2, fill=True, expand=True)

        label = fsui.HeadingLabel(self, _("Kickstart Versions"))
        self.layout2.add(label)#, expand=True, fill=True)
        self.layout2.add_spacer(10)

        hor_layout = fsui.HorizontalLayout()
        self.layout2.add(hor_layout, fill=True)

        self.kickstart_groups = []

        ver_layout = fsui.VerticalLayout()
        hor_layout.add(ver_layout, expand=True, fill=True)

        self.add_kickstart_group(ver_layout, "Amiga 500", "A500")
        ver_layout.add_spacer(10)
        self.add_kickstart_group(ver_layout, "Amiga 500+", "A500+")


        ver_layout = fsui.VerticalLayout()
        hor_layout.add(ver_layout, expand=True, fill=True)

        self.add_kickstart_group(ver_layout, "Amiga 600", "A600")
        ver_layout.add_spacer(10)
        self.add_kickstart_group(ver_layout, "Amiga 1200", "A1200")

        ver_layout = fsui.VerticalLayout()
        hor_layout.add(ver_layout, expand=True, fill=True)

        self.add_kickstart_group(ver_layout, "Amiga 1200/020", "A1200/020")
        ver_layout.add_spacer(10)
        self.add_kickstart_group(ver_layout, "Amiga 4000/040", "A4000/040")

    def add_kickstart_group(self, layout, title, model):
        group = KickstartStatusGroup(self, title, model)
        self.kickstart_groups.append(group)
        layout.add(group, fill=True)
