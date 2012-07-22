from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import fs_uae_launcher.fsui as fsui
from ..Config import Config
from ..IconButton import IconButton
from ..Warnings import Warnings
from ..Database import Database
from ..Amiga import Amiga
from ..I18N import _, ngettext

class ModelGroup(fsui.Group):

    def __init__(self, parent, with_more_button=True):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.HorizontalLayout()
        self.layout.padding_left = 20
        self.layout.padding_right = 20

        image = fsui.Image("fs_uae_launcher:res/model.png")
        self.image_view = fsui.ImageView(self, image)
        self.layout.add(self.image_view, valign=0.0)

        self.layout.add_spacer(20)

        self.vert_layout = fsui.VerticalLayout()
        self.layout.add(self.vert_layout, fill=True, expand=True)

        #self.hor_layout = fsui.HorizontalLayout()
        #self.vert_layout.add(self.hor_layout, fill=True, expand=True)

        label = fsui.HeadingLabel(self, _("Amiga Model"))
        self.vert_layout.add(label)#, expand=True, fill=True)
        self.vert_layout.add_spacer(10)

        self.hor_layout = fsui.HorizontalLayout()
        self.vert_layout.add(self.hor_layout, fill=True)

        model_titles = [x["title"] for x in Amiga.models]
        self.model_choice = fsui.Choice(self, model_titles)

        self.hor_layout.add(self.model_choice, expand=True)

        self.hor_layout.add_spacer(10)

        accuracy_levels = [_("Accurate"), _("Less Accurate"),
                _("Least Accurate")]

        self.accuracy_choice = fsui.Choice(self, accuracy_levels)
        self.hor_layout.add(self.accuracy_choice, expand=False)

        """
        if with_more_button:

            self.hor_layout.add_spacer(10)
            self.hds_button = IconButton(self, "hd_button.png")
            #self.hds_button.disable()
            self.hds_button.set_tooltip(_("Hard Drives"))
            ##self.hds_button = fsui.Button(self, _("Hard Drives"))
            self.hds_button.on_activate = self.on_hds_button
            self.hor_layout.add(self.hds_button, fill=True)

            self.hor_layout.add_spacer(10)

            #self.more_button = fsui.Button(self, _("More..."))
            #self.more_button.on_activate = self.on_more_button

            self.more_button = IconButton(self, "hardware_button.png")
            self.more_button.set_tooltip("More Amiga Hardware Options")
            self.more_button.on_activate = self.on_more_button

            self.hor_layout.add(self.more_button, fill=True)
        """

        # FIXME: should not need to call from here...
        Config.update_kickstart()

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config("amiga_model", Config.get("amiga_model"))
        self.on_config("accuracy", Config.get("accuracy"))

    def set_config_handlers(self):
        self.model_choice.on_change = self.on_model_change
        self.accuracy_choice.on_change = self.on_accuracy_change
        Config.add_listener(self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)

    def on_more_button(self):
        from .configui.ConfigDialog import ConfigDialog
        dialog = ConfigDialog(self.get_window(), ConfigDialog.HARDWARE)
        dialog.show_modal()
        dialog.destroy()

    def on_model_change(self):
        print("\non_model_change\n\n")
        index = self.model_choice.get_index()
        model = Amiga.models_config[index]
        Config.set("amiga_model", model)
        Config.update_kickstart()
        if Amiga.is_cd_based():
            from ..FloppyManager import FloppyManager
            FloppyManager.clear_all()
        else:
            from ..CDManager import CDManager
            CDManager.clear_all()

    def on_accuracy_change(self):
        index = self.accuracy_choice.get_index()
        Config.set("accuracy", str(1 - index))

    def on_config(self, key, value):
        if key == 'amiga_model':
            for i, config in enumerate(Amiga.models_config):
                if config == value:
                    self.model_choice.set_index(i)
                    break
            else:
                print("FIXME: could not set model")
        elif key == 'accuracy':
            index = 1 - int(value)
            self.accuracy_choice.set_index(index)

    def on_hds_button(self):
        from .configui.ConfigDialog import ConfigDialog
        dialog = ConfigDialog(self.get_window(), ConfigDialog.HARD_DRIVES)
        dialog.show_modal()
        dialog.destroy()
