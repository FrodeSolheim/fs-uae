from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ...I18N import _, ngettext
from ...Settings import Settings
from ...Signal import Signal
from ..PagedDialog import PagedDialog
from .AudioSettingsPage import AudioSettingsPage
#from .CustomSettingsPage import CustomSettingsPage
from .ExperimentalFeaturesPage import ExperimentalFeaturesPage
from .FilterSettingsPage import FilterSettingsPage
from .GameDatabaseSettingsPage import GameDatabaseSettingsPage
from .InputSettingsPage import InputSettingsPage
from .JoystickSettingsPage import JoystickSettingsPage
from .NetplaySettingsPage import NetplaySettingsPage
from .ScanSettingsPage import ScanSettingsPage
from .VideoSettingsPage import VideoSettingsPage

class SettingsDialog(PagedDialog):

    def __init__(self, parent, index=0):
        PagedDialog.__init__(self, parent, _("FS-UAE Launcher Settings"))

        self.add_page(_("Joystick Settings"), JoystickSettingsPage)
        self.add_page(_("Scan & Indexing"), ScanSettingsPage)
        self.add_page(_("Input Settings"), InputSettingsPage)
        self.add_page(_("Audio Settings"), AudioSettingsPage)
        self.add_page(_("Video Settings"), VideoSettingsPage)
        self.add_page(_("Filters & Scaling"), FilterSettingsPage)
        #self.add_page(_("OpenGL Settings"), OpenGLSettingsPage)
        self.add_page(_("Experimental Features"), ExperimentalFeaturesPage)
        if Settings.get("netplay_feature") == "1":
            self.add_page(_("Net Play Settings"), NetplaySettingsPage)
        if Settings.get("database_feature") == "1":
            self.add_page(_("Game Database"), GameDatabaseSettingsPage)
        #self.add_page(_("Custom Settings"), CustomSettingsPage)

        self.list_view.set_index(index)
        self.set_size((900, 540))
        self.center_on_parent()

    @classmethod
    def run(cls, parent, index=0):
        dialog = cls(parent, index)
        dialog.show_modal()
        dialog.close()
        Signal.broadcast("settings_updated")
