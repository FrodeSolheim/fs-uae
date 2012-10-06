from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..Settings import Settings
from .XMLControl import XMLControl
from .PickGameDialog import PickGameDialog
from ..I18N import _, ngettext

class EditorGroup(fsui.Group):

    def __init__(self, parent):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()
        #self.layout.padding_left = 20
        #self.layout.padding_right = 20

        self.layout.add_spacer(20)

        self.label = fsui.HeadingLabel(self, "Edit Config")
        self.layout.add(self.label)

        self.layout.add_spacer(10)

        horlayout = fsui.HorizontalLayout()
        self.layout.add(horlayout, fill=True)

        self.game_button = fsui.Button(self, "Pick Game")
        self.game_button.on_activate = self.on_game_button
        horlayout.add(self.game_button, fill=True)

        horlayout.add_spacer(10)
        horlayout.add_spacer(0, expand=True)

        self.save_button = fsui.Button(self, "Save")
        self.save_button.on_activate = self.on_save_button
        horlayout.add(self.save_button, fill=True)

        self.layout.add_spacer(14)

        self.config_xml_ctrl = XMLControl(self)
        self.layout.add(self.config_xml_ctrl, fill=True, expand=True)

        self.layout.add_spacer(20)

        self.game_xml_ctrl = XMLControl(self)
        self.layout.add(self.game_xml_ctrl, fill=True, expand=True)

        self.layout.add_spacer(20)

        Settings.add_listener(self)
        Config.add_listener(self)

    def on_config(self, key, value):
        if key == "x_game_xml_path":
            self.game_xml_ctrl.set_path(value)

    def on_setting(self, key, value):
        if key == "config_xml_path":
            #self.load_config_xml(value)
            self.config_xml_ctrl.set_path(value)

    def on_save_button(self):
        self.config_xml_ctrl.save()
        self.game_xml_ctrl.save()

    def on_game_button(self):
        dialog = PickGameDialog(self.get_window())
        if dialog.show_modal():
            game_info = dialog.get_game_info()
        else:
            game_info = None
        dialog.destroy()
        if game_info is None:
            return
        print(game_info)
        self.config_xml_ctrl.connect_game(game_info)
