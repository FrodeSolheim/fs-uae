from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ...Config import Config
from ...Warnings import Warnings
from ...Database import Database
from ...Amiga import Amiga
from ...FloppyManager import FloppyManager
from ...CDManager import CDManager
from ...I18N import _, ngettext
from .ConfigDialog import ConfigDialog
from .ConfigCheckBox import ConfigCheckBox


class ModelGroup(fsui.Group):

    # FIXME: remove with_more_button=True
    def __init__(self, parent, with_more_button=True):
        fsui.Group.__init__(self, parent)
        self.layout = fsui.VerticalLayout()

        heading_label = fsui.HeadingLabel(self, _("Amiga Model"))

        model_titles = [x["title"] for x in Amiga.models]
        self.model_choice = fsui.Choice(self, model_titles)

        self.accuracy_choice = fsui.Choice(self, [
                _("Accurate"),
                _("Less Accurate"),
                _("Least Accurate")])
        #self.ntsc_checkbox = fsui.CheckBox(self, "NTSC")
        self.ntsc_checkbox = ConfigCheckBox(self, "NTSC", "ntsc_mode")

        self.layout.add(heading_label, margin=10)
        self.layout.add_spacer(0)
        hori_layout = fsui.HorizontalLayout()
        self.layout.add(hori_layout, fill=True)
        hori_layout.add(self.model_choice, expand=True, margin=10)
        hori_layout.add(self.accuracy_choice, expand=False, margin=10)
        hori_layout.add(self.ntsc_checkbox, expand=False, margin=10)

        # FIXME: should not need to call from here...
        #Config.update_kickstart()

        self.initialize_from_config()
        self.set_config_handlers()

    def initialize_from_config(self):
        self.on_config("amiga_model", Config.get("amiga_model"))
        self.on_config("accuracy", Config.get("accuracy"))
        #self.on_config("ntsc", Config.get("ntsc"))

    def set_config_handlers(self):
        self.model_choice.on_change = self.on_model_change
        self.accuracy_choice.on_change = self.on_accuracy_change
        #self.ntsc_checkbox.on_change = self.on_ntsc_change
        Config.add_listener(self)

    def on_destroy(self):
        print("on_destroy")
        Config.remove_listener(self)

    def on_more_button(self):
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
            FloppyManager.clear_all()
        else:
            CDManager.clear_all()

    def on_accuracy_change(self):
        index = self.accuracy_choice.get_index()
        if index == 0:
            Config.set("accuracy", "")
        else:
            Config.set("accuracy", str(1 - index))

    #def on_ntsc_change(self):
    #    if self.ntsc_checkbox.is_checked():
    #        Config.set("ntsc_mode", "1")
    #    else:
    #        Config.set("ntsc_mode", "")

    def on_config(self, key, value):
        if key == "amiga_model":
            for i, config in enumerate(Amiga.models_config):
                if config == value:
                    self.model_choice.set_index(i)
                    break
            else:
                print("FIXME: could not set model")
        elif key == "accuracy":
            if not value:
                index = 0
            else:
                index = 1 - int(value)
            self.accuracy_choice.set_index(index)
        #elif key == "ntsc_mode":
        #    if value == "1":
        #        self.ntsc_checkbox.check(True)
        #    else:
        #        self.ntsc_checkbox.check(False)

    def on_hds_button(self):
        dialog = ConfigDialog(self.get_window(), ConfigDialog.HARD_DRIVES)
        dialog.show_modal()
        dialog.destroy()
