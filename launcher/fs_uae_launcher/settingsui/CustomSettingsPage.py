from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import hashlib
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from ..I18N import _, ngettext

class CustomSettingsPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.layout.add_spacer(580, 20)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True, expand=True)

        hor_layout.add_spacer(20)
        self.text_area = fsui.TextArea(self)
        self.text_area.set_min_height(400)
        self.text_area.set_text(self.get_initial_text())
        hor_layout.add(self.text_area, fill=True, expand=True)
        hor_layout.add_spacer(20)

        self.layout.add_spacer(20)

        #hor_layout = fsui.HorizontalLayout()
        #self.layout.add(hor_layout, fill=True)

        #hor_layout.add_spacer(20, expand=True)
        #self.close_button = fsui.Button(self, _("Close"))
        #self.close_button.on_activate = self.on_close_button
        #hor_layout.add(self.close_button)
        #hor_layout.add_spacer(20)

        #self.layout.add_spacer(20)
        #self.set_size(self.layout.get_min_size())
        #self.center_on_parent()
        self.get_window().add_close_listener(self.on_close_window)

    def on_close_window(self):
        self.update_settings()

    #def on_close_button(self):
    #    self.end_modal(0)

    def update_settings(self):
        text = self.text_area.get_text()
        keys = Settings.settings.keys()
        for key in keys:
            if key not in Settings.default_settings:
                del Settings.settings[key]

        for line in text.split(u"\n"):
            line = line.strip()
            if line.startswith("# You can write key = value pairs here"):
                continue
            parts = line.split("=", 1)
            if len(parts) == 2:
                key = parts[0].strip()
                if key in Settings.default_settings:
                    continue
                value = parts[1].strip()
                Settings.set(key, value)

    def get_initial_text(self):
        text = u"# You can write key = value pairs here to set FS-UAE options\n" \
                u"# for which there is no user interface yet, such as theme and\n" \
                u"# video sync options.\n\n" \
                u"# The options specified here will apply to all configurations.\n\n"
        keys = Settings.settings.keys()
        for key in sorted(keys):
            if key in Settings.default_settings:
                continue
            value = Settings.settings[key]
            text += u"{0} = {1}\n".format(key, value)
        return text
