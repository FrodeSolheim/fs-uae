from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import xml.etree.ElementTree
from xml.etree.cElementTree import ElementTree
import fs_uae_launcher.fsui as fsui
#from ..Database import Database
from ..I18N import _, ngettext
from .GameListView import GameListView

class PickGameDialog(fsui.Dialog):

    def __init__(self, parent):
        fsui.Dialog.__init__(self, parent, "Choose Game")
        self.info = {}
        self.layout = fsui.VerticalLayout(padding=20)
        self.text = fsui.TextField(self)
        self.text.on_change = self.on_text
        self.layout.add(self.text, fill=True)
        self.layout.add_spacer(20)
        self.view = GameListView(self)
        self.view.set_game_info = self.set_game_info
        self.view.set_min_width(600)
        self.view.set_min_height(300)
        self.layout.add(self.view, expand=True, fill=True)
        self.layout.add_spacer(20)

        self.link_field = fsui.TextField(self)
        self.layout.add(self.link_field, fill=True)
        self.layout.add_spacer(20)


        hlayout = fsui.HorizontalLayout()
        self.layout.add(hlayout, fill=True)
        hlayout.add_spacer(0, expand=True)
        self.cancel_button = fsui.Button(self, _("Cancel"))
        self.cancel_button.on_activate = self.on_cancel_button
        hlayout.add(self.cancel_button)
        hlayout.add_spacer(10)
        self.ok_button = fsui.Button(self, _("OK"))
        self.ok_button.on_activate = self.on_ok_button
        hlayout.add(self.ok_button)

        self.text.focus()
        self.set_size(self.layout.get_min_size())
        self.center_on_parent()

    def set_game_info(self, info):
        self.info = info
        tree = ElementTree()
        tree.parse(info["path"])
        link_node = tree.find("link")
        if link_node is not None:
            self.link_field.set_text(link_node.text.strip())
        else:
            self.link_field.set_text("")

    def on_text(self):
        self.view.search(self.text.get_text())

    def get_game_info(self):
        return self.info

    def get_game_uuid(self):
        return self.info.get("uuid")

    def on_ok_button(self):
        self.end_modal(bool(self.info.get("uuid")))

    def on_cancel_button(self):
        self.end_modal(False)
