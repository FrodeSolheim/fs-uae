from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import fs_uae_launcher.fsui as fsui
from ..Amiga import Amiga
#from ..Config import Config
from ..Database import Database
from ..I18N import _, ngettext
#from ..Settings import Settings
from ..Config import Config
from ..Signal import Signal
from .SetupDialog import SetupDialog
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
        self.update_web_url = ""
        self.update_version = ""
        self.toolbar_mode = toolbar_mode
        self.padding_top = padding_top
        self.padding_bottom = padding_bottom

        self.chip_memory_warning = None
        self.kickstarts_missing = False
        self.update_available = False

        self.update_available_icon = fsui.Image("fs_uae_launcher:res/"
                "update_available_32.png")
        self.warning_icon = fsui.Image("fs_uae_launcher:res/"
                "warning_32.png")
        self.kickstarts_missing_icon = self.warning_icon

        self.check_kickstarts()
        #Config.add_listener(self)
        #Settings.add_listener(self)
        Signal.add_listener("update_available", self)
        Signal.add_listener("scan_done", self)
        Signal.add_listener("config", self)

    def on_destroy(self):
        #Config.remove_listener(self)
        #Settings.remove_listener(self)
        Signal.remove_listener("config", self)
        Signal.remove_listener("scan_done", self)
        Signal.remove_listener("update_available", self)

#    def on_config(self, key, value):
#        pass
#
#    def on_setting(self, key, value):
#        pass

    def on_config(self, key, value):
        #print("InfoPanel.on_config", key, value)
        if key in ["amiga_model", "chip_memory"]:
            amiga_model = Config.get("amiga_model")
            try:
                chip_memory = int(Config.get("chip_memory"))
            except:
                chip_memory = ""
            if chip_memory and chip_memory < 2048 and \
                    amiga_model in ["A1200", "A1200/020", "A4000/040"]:
                new_chip_memory_warning = [_("Configuration Warning"),
                        _("{amiga_model} with < 2 MB chip memory"
                        "".format(amiga_model=amiga_model))]
            else:
                new_chip_memory_warning = None
            if new_chip_memory_warning != self.chip_memory_warning:
                self.chip_memory_warning = new_chip_memory_warning
                self.refresh()

    def on_scan_done_signal(self):
        print("InfoPanel.on_scan_done_signal")
        self.check_kickstarts()

    def check_kickstarts(self):
        # FIXME: instead of this check, check if x_kickstart_sha1 is set
        # properly when amiga_model / x_kickstart_sha1 changes, so you''
        # only get a warning for the Amiga model in question.
        ok = True
        database = Database.get_instance()
        amiga = Amiga.get_model_config("A500")
        for sha1 in amiga["kickstarts"]:
            if database.find_file(sha1=sha1):
                ok = False
                break
        self.kickstarts_missing = ok
        #self.kickstarts_missing = True
        self.refresh()

    def on_update_available_signal(self, version, web_url):
        self.update_available = True
        self.update_version = version
        self.update_web_url = web_url
        self.refresh()

    def on_left_up(self):
        if self.kickstarts_missing:
            dialog = SetupDialog(self.get_window())
            dialog.show_modal()
            dialog.destroy()
        elif self.update_available:
            import webbrowser
            webbrowser.open(self.update_web_url)

    def on_paint(self):
        dc = self.create_dc()
        if not self.toolbar_mode:
            TabPanel.draw_background(self, dc)

        if self.chip_memory_warning:
            self.draw_notification(dc, self.warning_icon,
                    *self.chip_memory_warning)
        elif self.kickstarts_missing:
            self.draw_kickstarts_missing_notification(dc)
        elif self.update_available:
            self.draw_update_available_notification(dc)

    def draw_update_available_notification(self, dc):
        self.draw_notification(dc, self.update_available_icon,
                _("Update available ({version})").format(
                version=self.update_version),
                _("Click here to download"))

    def draw_kickstarts_missing_notification(self, dc):
        self.draw_notification(dc, self.kickstarts_missing_icon,
                _("Kickstart ROMs are missing"),
                _("Click here to import kickstarts"))

    def draw_notification(self, dc, icon, text1, text2):
        available_height = self.size[1]
        available_height -= self.padding_top + self.padding_bottom

        y = self.padding_top + (available_height - icon.size[1]) // 2

        if fsui.System.macosx:
            rtl = False
        else:
            rtl = True

        if rtl:
            right_x = self.size[0]
            icon_x = right_x - icon.size[0] - 10
            dc.draw_image(icon, icon_x, y)
            right_x = icon_x - 20
            font = dc.get_font()
            font.set_bold(True)
            dc.set_font(font)
            tw, th = dc.measure_text(text1)
            lines_height = th * 2
            y = self.padding_top + (available_height - lines_height) // 2
            dc.draw_text(text1, right_x - tw, y)
            y += th
            font.set_bold(False)
            dc.set_font(font)
            tw, th = dc.measure_text(text2)
            dc.draw_text(text2, right_x - tw, y)
        else:
            dc.draw_image(icon, 0, y)
            x = 50
            font = dc.get_font()
            font.set_bold(True)
            dc.set_font(font)
            tw, th = dc.measure_text(text1)
            lines_height = th * 2
            y = self.padding_top + (available_height - lines_height) // 2
            #dc.draw_text(text1, right_x - tw, y)
            dc.draw_text(text1, x, y)
            y += th
            font.set_bold(False)
            dc.set_font(font)
            tw, th = dc.measure_text(text2)
            #dc.draw_text(text, right_x - tw, y)
            dc.draw_text(text2, x, y)
