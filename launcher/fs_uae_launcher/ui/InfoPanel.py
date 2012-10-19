from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ..I18N import _, ngettext
from ..Signal import Signal
from .Skin import Skin
from .TabPanel import TabPanel

if fsui.System.macosx:
    base_class = fsui.Control
else:
    base_class= fsui.Panel

class InfoPanel(base_class):

    def __init__(self, parent, toolbar_mode=False, padding_top=0,
            padding_bottom=0):
        base_class.__init__(self, parent, paintable=True)
        Skin.set_background_color(self)
        self.mode = ""
        self.update_web_url = ""
        self.update_version = ""
        self.toolbar_mode = toolbar_mode
        self.padding_top = padding_top
        self.padding_bottom = padding_bottom
        Signal.add_listener("update_available", self)

    def on_destroy(self):
        Signal.remove_listener("update_available", self)

    def on_update_available_signal(self, version, web_url):
        self.mode = "UPDATE"
        self.update_version = version
        self.update_web_url = web_url
        self.refresh()

    def on_left_up(self):
        if self.mode == "UPDATE":
            import webbrowser
            webbrowser.open(self.update_web_url)

    def on_paint(self):
        dc = self.create_dc()
        font = dc.get_font()
        font.set_bold(True)
        dc.set_font(font)

        if not self.toolbar_mode:
            TabPanel.draw_background(self, dc)
        if self.mode == "UPDATE":
            self.draw_update_notification(dc)

    def draw_update_notification(self, dc):
        available_height = self.size[1]
        available_height -= self.padding_top + self.padding_bottom
        x = 10
        #right_x = self.size[0] - 10
        
        text = ("Update ({version}) is available".format(
                version=self.update_version))
        tw, th = dc.measure_text(text)
        lines_height = th * 2
        y = self.padding_top + (available_height - lines_height) // 2
        
        #dc.draw_text(text, right_x - tw, y)
        dc.draw_text(text, x, y)
        y += th
        text = _("Click here to download")
        tw, th = dc.measure_text(text)
        #dc.draw_text(text, right_x - tw, y)
        dc.draw_text(text, x, y)

