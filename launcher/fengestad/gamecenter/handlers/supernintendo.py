from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
import pkg_resources
from fengestad import fs
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController, Option
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class SuperNintendoController(GameController):

    def init(self):
        self.controller1_option = InputOption(1)
        self.options.append(self.controller1_option)
        self.controller2_option = InputOption(2)
        self.options.append(self.controller2_option)

    def init_input(self):
        self.inputs = [
            self.create_input(name="Controller 1",
                    type="supernintendo",
                    description="Controller"),
            self.create_input(name="Controller 2",
                    type="supernintendo",
                    description="Controller"),
        ]
        #if fs.windows:
        #    self.input_device_order = "DINPUT8"
        #self.input_mapping_multiple = False

    def prepare(self):
        print("SuperNintendoController.prepare")
        try:
            self.firmware_dir = self.prepare_firmware(
                    "Super Nintendo Firmware")
        except Exception:
            self.firmware_dir = None

    def configure(self):
        self.args = []
        if fs.windows:
            from win32com.shell import shell, shellcon
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_APPDATA, 0, 0)
            config_dir = os.path.join(path, "bsnes")
        else:
            config_dir = os.path.join(fs.get_home_dir(), ".config", "bsnes")
        for name in ["bsnes.cfg", "geometry.cfg", "paths.cfg"]:
            p = os.path.join(config_dir, name)
            if os.path.exists(p):
                os.remove(p)
        settings_path = os.path.join(config_dir, "bsnes", "bsnes.cfg")
        input_path = os.path.join(config_dir, "bsnes", "bsnes.cfg")
        if not os.path.exists(os.path.dirname(settings_path)):
            os.makedirs(os.path.dirname(settings_path))
        with open(settings_path, "w") as f:
            self.set_graphics_options(f)
        #with open(input_path, "w") as f:
            self.write_input_config(f)
        # bsnes saves states and ram to the dir containing the rom
        temp_dir = self.context.temp.dir("bsnes")
        if self.context.game.file.endswith(".zip"):
            GameCenterUtil.unpack(self.context.game.file, temp_dir)
            for item in os.listdir(temp_dir):
                if item.endswith(".sfc"):
                    game_file = os.path.join(temp_dir, item)
                    break
        else:
            game_file = os.path.join(temp_dir,
                    os.path.basename(self.context.game.file))
            shutil.copyfile(self.context.game.file, game_file)
        if self.firmware_dir:
            for item in os.listdir(self.firmware_dir):
                path = os.path.join(self.firmware_dir, item)
                shutil.copyfile(path, os.path.join(temp_dir, item))
        self.args.append(game_file)
        self.changes = ChangeHandler(temp_dir)
        self.changes.init(self.context.game.state_dir)

    def run(self):
        emulator = "fs-snes"
        return self.run_emulator(emulator)

    def cleanup(self):
        self.changes.update(self.context.game.state_dir)

    def get_supported_filters(self):
        supported = [{
                "name": "2x",
                "video_filter": "Pixellate2x.filter",
            }, {
                "name": "none",
                "video_filter": "",
            }, {
                "name": "ntsc",
                "video_filter": "NTSC-RF.filter",
            }, {
                "name": "scale2x",
                "video_filter": "Scale2x.filter",
            }, {
                "name": "hq2x",
                "video_filter": "HQ2x.filter",
            }
        ]
        
        #filters_dir = pkg_resources.resource_filename(
        #        "fengestad.program.bsnes", "filters")
        # FIXME
        filters_dir = ""
        #plugin = pyapp.plugins.get_plugin("no.fengestad.emulator.bsnes")
        for item in supported:
            if not item["video_filter"]:
                continue
            item["video_filter"] = os.path.join(filters_dir,
                    item["video_filter"])
        return supported

    def set_graphics_options(self, f):
        if self.get_option("fullscreen"):
            # FIXME: CONFIG VALUE
            self.args.append("--fullscreen")

        if self.configure_vsync():
            self.args.append("--vsync")

    def write_input_config(self, f):
        input_mapping = [{
                "a": "input.port1.gamepad.a",
                "b": "input.port1.gamepad.b",
                "x": "input.port1.gamepad.x",
                "y": "input.port1.gamepad.y",
                "l": "input.port1.gamepad.l",
                "r": "input.port1.gamepad.r",
                "up": "input.port1.gamepad.up",
                "down": "input.port1.gamepad.down",
                "left": "input.port1.gamepad.left",
                "right": "input.port1.gamepad.right",
                "select": "input.port1.gamepad.select",
                "start": "input.port1.gamepad.start",
            }, {
                "a": "input.port2.gamepad.a",
                "b": "input.port2.gamepad.b",
                "x": "input.port2.gamepad.x",
                "y": "input.port2.gamepad.y",
                "l": "input.port2.gamepad.l",
                "r": "input.port2.gamepad.r",
                "up": "input.port2.gamepad.up",
                "down": "input.port2.gamepad.down",
                "left": "input.port2.gamepad.left",
                "right": "input.port2.gamepad.right",
                "select": "input.port2.gamepad.select",
                "start": "input.port2.gamepad.start",
            }]

        for i, input in enumerate(self.inputs):
            mapper = InputMapper(input, input_mapping[i])
            for key, value in mapper.iteritems():
                f.write("{key} = \"{value}\"\n".format(
                        key=key, value=value))
        f.write("input.hotkeys.general.exitEmulator = \"KB0::Escape\"\n")


class InputOption(Option):

    def __init__(self, number):
        Option.__init__(self)
        self.priority = 0.1 * number
        self.title = "Gamepad"
        self.subtitle = "Controller Port {0}".format(number)
        self.value = "gamepad"

    def activate(self):
        print("InputItem.activate")
        return [
            {"title": "Nothing", "subtitle": "", "value": ""},
            {"title": "Gamepad", "subtitle": "", "value": "gamepad"}
        ]


class InputMapper(GameController.InputMapper):

    def axis(self, axis, positive):
        if positive:
            return "JP{joy_index}::Axis{axis}.Hi".format(
                    joy_index=self.device.index, axis=axis)
        else:
            return "JP{joy_index}::Axis{axis}.Lo".format(
                    joy_index=self.device.index, axis=axis)

    def hat(self, hat, direction):
        dir_str = {
            "left": "Left",
            "right": "Right",
            "up": "Up",
            "down": "Down",
        }[direction]
        return "JP{joy_index}::Hat{hat}.{dir}".format(
                joy_index=self.device.index, hat=hat, dir=dir_str)
        
    def button(self, button):
        return "JP{joy_index}::Button{button}".format(
                joy_index=self.device.index, button=button)

    def key(self, key):
        return "KB0::" + self.keycodes[key.sdl_name[5:].upper()]

    keycodes = {
          "ESCAPE": "Escape",
          "F1": "F1",
          "F2": "F2",
          "F3": "F3",
          "F4": "F4",
          "F5": "F5",
          "F6": "F6",
          "F7": "F7",
          "F8": "F8",
          "F9": "F9",
          "F10": "F10",
          "F11": "F11",
          "F12": "F12",
          "PRINT": "PrintScreen",
          "SCROLLOCK": "ScrollLock",
          "PAUSE": "Pause",
          "": "Tilde",
          "1": "Num1",
          "2": "Num2",
          "3": "Num3",
          "4": "Num4",
          "5": "Num5",
          "6": "Num6",
          "7": "Num7",
          "8": "Num8",
          "9": "Num9",
          "0": "Num0",
          "": "Dash",
          "": "Equal",
          "BACKSPACE": "Backspace",
          "INSERT": "Insert",
          "DELETE": "Delete",
          "HOME": "Home",
          "END": "End",
          "PAGEUP": "PageUp",
          "PAGEDOWN": "PageDown",
          "A": "A",
          "B": "B",
          "C": "C",
          "D": "D",
          "E": "E",
          "F": "F",
          "G": "G",
          "H": "H",
          "I": "I",
          "J": "J",
          "K": "K",
          "L": "L",
          "M": "M",
          "N": "N",
          "O": "O",
          "P": "P",
          "Q": "Q",
          "R": "R",
          "S": "S",
          "T": "T",
          "U": "U",
          "V": "V",
          "W": "W",
          "X": "X",
          "Y": "Y",
          "Z": "Z",
          "LEFTBRACKET": "LeftBracket",
          "RIGHTBRACKET": "RightBracket",
          "BACKSLASH": "Backslash",
          "SEMICOLON": "Semicolon",
          "QUOTE": "Apostrophe",
          "COMMA": "Comma",
          "PERIOD": "Period",
          "SLASH": "Slash",
          "KP1": "Keypad1",
          "KP2": "Keypad2",
          "KP3": "Keypad3",
          "KP4": "Keypad4",
          "KP5": "Keypad5",
          "KP6": "Keypad6",
          "KP7": "Keypad7",
          "KP8": "Keypad8",
          "KP9": "Keypad9",
          "KP0": "Keypad0",
          "KP_PERIOD": "Point",
          "KP_ENTER": "Enter",
          "KP_PLUS": "Add",
          "KP_MINUS": "Subtract",
          "KP_MULTIPLY": "Multiply",
          "KP_DIVIDE": "Divide",
          "NUMLOCK": "NumLock",
          "CAPSLOCK": "CapsLock",
          "UP": "Up",
          "DOWN": "Down",
          "LEFT": "Left",
          "RIGHT": "Right",
          "TAB": "Tab",
          "RETURN": "Return",
          "SPACE": "Spacebar",
          "MENU": "Menu",
          "LSHIFT": "Shift",
          "LCTRL": "Control",
          "LALT": "Alt",
          "LSUPER": "Super",
        }
