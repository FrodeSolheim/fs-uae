from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals
from fs_uae_launcher.DeviceManager import DeviceManager

from fsui import Image
from ...Config import Config
from .StatusElement import StatusElement

WARNING_LEVEL = 0
NOTICE_LEVEL = 1
JOYSTICK_KEYS = ["joystick_port_0", "joystick_port_1", "joystick_port_2",
                 "joystick_port_3", "joystick_port_0_mode",
                 "joystick_port_1_mode", "joystick_port_2_mode",
                 "joystick_port_3_mode"]


class WarningsElement(StatusElement):

    def __init__(self, parent):
        StatusElement.__init__(self, parent)
        self.warning_icon = Image("fs_uae_launcher:res/16/warning_2.png")
        self.notice_icon = Image("fs_uae_launcher:res/16/information.png")

        self.icons = [
            self.warning_icon,
            self.notice_icon,
        ]
        self.game_notice = ""
        self.variant_notice = ""
        self.joy_emu_conflict = ""
        self.using_joy_emu = False
        self.warnings = []
        Config.add_listener(self)
        self.on_config("x_game_notice", Config.get("x_game_notice"))
        self.on_config("x_variant_notice", Config.get("x_variant_notice"))
        self.on_config("x_joy_emu_conflict", Config.get("x_joy_emu_conflict"))
        self.on_config("joystick_port_0", Config.get("joystick_port_0"))

    def on_destroy(self):
        Config.remove_listener(self)

    def rebuild_warnings(self):
        self.warnings = []
        if self.using_joy_emu and self.joy_emu_conflict:
            self.warnings.append((WARNING_LEVEL, self.joy_emu_conflict))
        for name in ["variant_notice", "game_notice"]:
            value = getattr(self, name)
            if not value:
                continue
            if value.startswith("WARNING: "):
                level = WARNING_LEVEL
                message = value[9:]
            else:
                level = NOTICE_LEVEL
                message = value
            self.warnings.append((level, message))
        self.warnings.sort()

    def on_config(self, key, value):
        if key == "x_game_notice":
            if value != self.game_notice:
                self.game_notice = value
                self.rebuild_warnings()
                self.refresh()
        elif key == "x_variant_notice":
            if value != self.variant_notice:
                self.variant_notice = value
                self.rebuild_warnings()
                self.refresh()
        elif key == "x_joy_emu_conflict":
            print("\n\n\nGOT x_joy_emu_conflict\n\n\n")
            if value != self.joy_emu_conflict:
                self.joy_emu_conflict = value
                self.rebuild_warnings()
                self.refresh()
        elif key in JOYSTICK_KEYS:
            prev_value = self.using_joy_emu
            devices = DeviceManager.get_devices_for_ports(Config)
            for device in devices:
                if device.id == "keyboard":
                    self.using_joy_emu = True
                    break
            else:
                self.using_joy_emu = False
            if prev_value != self.using_joy_emu:
                self.rebuild_warnings()
                self.refresh()

    def paint_element(self, dc):
        dc.set_font(dc.get_font())
        x = 6
        w, h = self.size
        for level, warning in self.warnings:
            icon = self.icons[level]
            dc.draw_image(icon, x, 6)
            x += 16 + 6
            tw, th = dc.measure_text(warning)
            dc.draw_text(warning, x, (h - th) / 2)
            x += tw + 6 + 16
