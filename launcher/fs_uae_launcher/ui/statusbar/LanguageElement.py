from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from fsui import ImageView
from ...Config import Config
from .StatusElement import StatusElement


class LanguageElement(StatusElement):

    def __init__(self, parent, language, icon, tool_tip=""):
        StatusElement.__init__(self, parent)
        self.set_min_width(32)
        #self.image_view = ImageView(self, icon)
        #self.image_view.set_position((8, 6 + 1))
        self.icon = icon
        self.language = language
        self.languages = ""
        self.hide()
        if tool_tip:
            self.set_tooltip(tool_tip)

        Config.add_listener(self)
        self.on_config("languages", Config.get("languages"))

    def on_destroy(self):
        Config.remove_listener(self)

    def on_config(self, key, value):
        if key == "languages":
            if value != self.languages:
                self.languages = value
                if self.language in value:
                    self.show()
                else:
                    self.hide()
