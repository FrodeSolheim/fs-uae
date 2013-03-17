from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import webbrowser
import fs_uae_launcher.fsui as fsui
from .IconButton import IconButton

class HelpButton(IconButton):
    def __init__(self, parent, url):
        IconButton.__init__(self, parent, "help_16.png")
        self.url = url

    def on_activate(self):
        webbrowser.open(self.url)
