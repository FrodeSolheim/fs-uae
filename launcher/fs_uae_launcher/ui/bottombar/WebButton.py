from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import webbrowser
from fsui import Image, Panel, Color
from ...Config import Config
from ...I18N import _
from ..Skin import Skin


class WebButton(Panel):

    def __init__(self, parent, icon=None):
        if icon is not None:
            self.icon = icon
        else:
            self.icon = Image("fs_uae_launcher:res/16/world.png")
        Panel.__init__(self, parent, paintable=True)
        #self.set_tooltip(tooltip)
        Config.add_listener(self)
        self.on_config("variant_rating", "")

    def on_destroy(self):
        Config.remove_listener(self)
        pass

    def get_min_width(self):
        return 32

    def get_min_height(self):
        return 24

    def get_url(self):
        variant_uuid = Config.get("variant_uuid", "")
        if not variant_uuid:
            return
        return "http://oagd.net/game/{0}".format(variant_uuid)

    def on_left_down(self):
        url = self.get_url()
        if url:
            webbrowser.open(url)

    def on_config(self, key, value):
        if key == "variant_uuid":
            if value:
                if not self.is_enabled():
                    self.enable()
                    #self.refresh()
                    self.show()
                    self.set_hand_cursor()
            else:
                if self.is_enabled():
                    self.disable()
                    self.hide()
                    #self.refresh()
                    #self.set_default_cursor()

    def on_paint(self):
        dc = self.create_dc()
        dc.clear(Skin.get_background_color())
        if self.is_enabled():
            dc.draw_image(self.icon, 8, 4)
        else:
            pass
