from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import hashlib
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..I18N import _, ngettext

class CustomOptionsPage(fsui.Panel):

    def __init__(self, parent):
        fsui.Panel.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        self.layout.add_spacer(20)

        hor_layout = fsui.HorizontalLayout()
        self.layout.add(hor_layout, fill=True, expand=True)

        hor_layout.add_spacer(20)
        self.text_area = fsui.TextArea(self)
        self.text_area.set_min_height(400)
        self.text_area.set_text(self.get_initial_text())
        hor_layout.add(self.text_area, fill=True, expand=True)
        hor_layout.add_spacer(20)

        self.layout.add_spacer(20)

        self.get_window().add_close_listener(self.on_close_window)

    def on_close_window(self):
        self.update_config()

    def on_close_button(self):
        self.end_modal(0)

    def update_config(self):
        text = self.text_area.get_text()
        keys = Config.config.keys()
        for key in keys:
            if key not in Config.default_config:
                del Config.config[key]

        for line in text.split(u"\n"):
            line = line.strip()
            if line.startswith("# You can write key = value pairs here"):
                continue
            parts = line.split("=", 1)
            if len(parts) == 2:
                key = parts[0].strip()
                if key in Config.no_custom_config:
                    continue
                value = parts[1].strip()
                Config.set(key, value)

    def get_initial_text(self):
        text = u"# You can write key = value pairs here to set FS-UAE options\n" \
                u"# for which there is no user interface yet.\n\n" \
                u"# The options specified here will apply to this configuration\n" \
                u"# only.\n\n"
        keys = Config.config.keys()
        for key in sorted(keys):
            if key in Config.no_custom_config:
                continue
            value = Config.config[key]
            text += u"{0} = {1}\n".format(key, value)
        return text
