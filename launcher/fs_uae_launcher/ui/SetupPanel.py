from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import traceback
import fs_uae_launcher.fsui as fsui
from ..Amiga import Amiga
from ..Config import Config
from ..Database import Database
from ..Settings import Settings
from ..I18N import _, ngettext
from .imports.ImportGroup import ImportGroup
from .ScanKickstartGroup import ScanKickstartGroup
from .Skin import Skin

class SetupPanel(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        Skin.set_background_color(self)
        self.layout = fsui.VerticalLayout()

        self.scan_kickstart_group = ScanKickstartGroup(self)
        self.import_rom_group = ImportGroup(self)
        self.import_af_group = ImportGroup(self, ImportGroup.AMIGA_FOREVER)

        self.layout.add(self.scan_kickstart_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        self.layout.add(self.import_rom_group, fill=True)
        self.layout.add_spacer(Skin.EXTRA_GROUP_MARGIN)
        self.layout.add(self.import_af_group, fill=True)

    def should_be_automatically_opened(self):
        database = Database.get_instance()
        amiga = Amiga.get_model_config("A500")
        for sha1 in amiga["kickstarts"]:
            if database.find_file(sha1=sha1):
                return False
        return True
