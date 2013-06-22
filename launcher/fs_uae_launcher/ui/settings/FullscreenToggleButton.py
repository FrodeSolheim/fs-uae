from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fsui as fsui
from ...I18N import _, ngettext
from ...Settings import Settings

class FullscreenToggleButton(fsui.ImageButton):

    def __init__(self, parent):
        self.windowed_icon = fsui.Image("fs_uae_launcher:res/windowed_16.png")
        self.fullscreen_icon = fsui.Image(
                "fs_uae_launcher:res/fullscreen_16.png")
        fsui.ImageButton.__init__(self, parent, self.windowed_icon)
        self.set_tooltip(_("Toggle Between Windowed and Full-Screen Mode"))
        self.set_min_width(40)
        self.fullscreen_mode = False
        self.on_setting("fullscreen", Settings.get("fullscreen"))
        Settings.add_listener(self)

    def on_destroy(self):
        Settings.remove_listener(self)

    def on_setting(self, key, value):
        if key == "fullscreen":
            if value == "1":
                self.fullscreen_mode = True
                self.set_image(self.fullscreen_icon)
            else:
                self.fullscreen_mode = False
                self.set_image(self.windowed_icon)

    def on_activate(self):
        if self.fullscreen_mode:
            Settings.set("fullscreen", "0")
        else:
            Settings.set("fullscreen", "1")
