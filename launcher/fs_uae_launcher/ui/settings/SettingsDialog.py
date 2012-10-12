from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...I18N import _, ngettext
from ...Signal import Signal
from ..PagedDialog import PagedDialog
#from .HardwarePage import HardwarePage
#from .HardDrivesPage import HardDrivesPage
from .JoystickSettingsPage import JoystickSettingsPage
from .CustomSettingsPage import CustomSettingsPage

class SettingsDialog(PagedDialog):

    #HARDWARE = 0
    #INPUT = 1
    #HARD_DRIVES = 2
    #CUSTOM_OPTIONS = 3

    def __init__(self, parent, index=0):
        PagedDialog.__init__(self, parent, _("Configuration"))

        #self.add_page(_("Hardware"), HardwarePage)
        self.add_page(_("Joystick Settings"), JoystickSettingsPage)
        #self.add_page(_("Hard Drives"), HardDrivesPage)
        self.add_page(_("Custom Settings"), CustomSettingsPage)

        self.list_view.set_index(index)
        self.set_size((800, 540))
        self.center_on_parent()

    @classmethod
    def run(cls, parent, index=0):
        dialog = cls(parent, index)
        dialog.show_modal()
        dialog.close()
        Signal.broadcast("settings_updated")
