from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsui import ImageView, Image, Label, HorizontalLayout
from ...Config import Config
from ...I18N import _
from .StatusElement import StatusElement


class PlayersElement(StatusElement):

    def __init__(self, parent):
        StatusElement.__init__(self, parent)
        self.icon = Image("fs_uae_launcher:res/16/user.png")
        self.text = _("N/A")
        self.active = False

        self.players = ""

        Config.add_listener(self)
        self.on_config("players", Config.get("players"))

    def on_destroy(self):
        Config.remove_listener(self)

    def on_config(self, key, value):
        if key == "players":
            if value != self.players:
                self.players = value
                if value:
                    self.text = value
                else:
                    self.text =  _("N/A")
                self.active = bool(value)
                self.refresh()

    def get_min_width(self):
        return 96
