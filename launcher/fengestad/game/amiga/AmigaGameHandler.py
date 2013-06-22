from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import shutil
import subprocess
#import pkg_resources
from fengestad import fs

from fsgs import fsgs
from fsgs.GameChangeHandler import GameChangeHandler
from fsgs.amiga.FSUAE import FSUAE
from fsgs.amiga.LaunchHandler import LaunchHandler
from fsgs.amiga.ValueConfigLoader import ValueConfigLoader

from fengestad.gamecenter.GameHandler import GameHandler
from fengestad.gamecenter.resources import resources, logger, ngettext, _

class AmigaGameHandler(GameHandler):

    def init(self):
        print("AmigaGameHandler.init")

        loader = ValueConfigLoader(self.context.game.variant_uuid)
        loader.load_values(self.context.game.config)
        self.amiga_config = loader.get_config()

        self.port1_option = InputOption(
            1, self.context.game.config.get("port1", "joystick"))
        #self.options.append(self.port1_option)

        self.port0_option = InputOption(
            0, self.context.game.config.get("port0", "mouse"))
        #self.options.append(self.port0_option)

    def init_input(self):
        print("AmigaGameHandler.init_input")
        mapping = {
            "joystick": ("amiga", "Joystick"),
            "mouse": ("amiga_mouse", "Mouse"),
            "": ("", "Nothing")
        }
        self.inputs = [
            self.create_input(name="Joystick Port 1",
                    type=mapping[self.port1_option.value][0],
                    description=mapping[self.port1_option.value][1]),
            self.create_input(name="Joystick Port 0",
                    type=mapping[self.port0_option.value][0],
                    description=mapping[self.port0_option.value][1])
        ]

    def prepare(self):
        print("AmigaGameHandler.prepare")
        
        self.temp_dir = self.context.get_temp_dir("amiga-temp")
        #self.change_handler = GameChangeHandler(self.temp_dir)

        #self.firmware_dir = self.prepare_firmware("Amiga Firmware")

        model = self.amiga_config.get("amiga_model")
        if model.startswith("CD32"):
            platform = "CD32"
        elif model == "CDTV":
            platform = "CDTV"
        else:
            platform = "Amiga"
        #name = Settings.get("config_name")
        name = self.context.game.name
        #uuid = Config.get("x_game_uuid")
        #uuid = None
        
        from fsgs.SaveStateHandler import SaveStateHandler
        save_state_handler = SaveStateHandler(fsgs, name, platform,
                self.context.game.variant_uuid)
        
        self.launch_handler = LaunchHandler(fsgs, name, self.amiga_config,
                save_state_handler)

        #self.change_handler.init(self.context.get_game_state_dir(),
        #        ignore=["*.uss", "*.sdf"])

        self.launch_handler.config["joystick_port_0"] = \
                self.inputs[1].device_id
        self.launch_handler.config["joystick_port_1"] = \
                self.inputs[0].device_id

        if self.get_option("fullscreen"):
            self.launch_handler.config["fullscreen"] = "1"
        #else:
        #    self.launch_handler.config["fullscreen"] = "0"

        self.launch_handler.prepare()

    def configure(self):
        print("AmigaGameHandler.configure")


        #temp_dir = self.context.temp.dir("amiga-config")
        #config_file = os.path.join(temp_dir, "amiga-config.fs-uae")
        #with open(config_file, "wb") as f:
        #    #self._configure_emulator(f)
        #    self.launch_handler.write_config(f)
        #    self.write_additional_config(f)#
        #
        ##self.args.extend(["--config", config_file])
        #self.args.extend([config_file])
        

    def write_additional_config(self, f):
        #if self.get_option("fullscreen"):
        #    f.write("fullscreen = 1\n")

        #if self.configure_vsync():
        #    f.write("video_sync = full\n")
        #else:
        #    f.write("video_sync = none\n")

        if self.get_option("fsaa"):
            f.write("fsaa = {0}\n".format(str(self.get_option("fsaa"))))

    def run(self):
        print("AmigaGameHandler.run")
        #self.on_progress(_("Starting FS-UAE..."))
        config = self.launch_handler.create_config()
        process, self.temp_config_file = FSUAE.start_with_config(config)
        #process.wait()
        #print("LaunchHandler.start is done")
        #print("removing", config_file)
        #try:
        #    os.remove(config_file)
        #except Exception:
        #    print("could not remove config file", config_file)
        return process

    def cleanup(self):
        print("removing", self.temp_config_file)
        try:
            os.remove(self.temp_config_file)
        except Exception:
            print("could not remove config file", self.temp_config_file)

        self.launch_handler.update_changes()
        self.launch_handler.cleanup()
        #self.change_handler.update(self.context.get_game_state_dir())

    def get_supported_filters(self):
        supported = []
        """
        supported = [{
                "name": "2x",
                "gfx_filter": "none",
                "line_double": True,
                "lores": False,
            }, {
                "name": "none",
                "gfx_filter": "none",
                "line_double": False,
                "lores": False,
            }, {
                "name": "pal",
                "gfx_filter": "pal",
                "line_double": True,
                "lores": False,
            }, {
                "name": "scale2x",
                "gfx_filter": "scale2x",
                "line_double": False,
                "lores": True,
            }, {
                "name": "hq2x",
                "gfx_filter": "hq2x",
                "line_double": False,
                "lores": True,
            }
        ]
        """
        return supported

    def set_input_options(self, f):
        # FIXME

        input_mapping = [{
                "1": "JOY2_FIRE_BUTTON",
                "2": "JOY2_2ND_BUTTON",
                "3": "JOY2_3RD_BUTTON",
                "left": "JOY2_LEFT",
                "right": "JOY2_RIGHT",
                "up": "JOY2_UP",
                "down": "JOY2_DOWN",
            }, {
                "1": "JOY1_FIRE_BUTTON",
                "2": "JOY1_2ND_BUTTON",
                "3": "JOY1_3RD_BUTTON",
                "left": "JOY1_LEFT",
                "right": "JOY1_RIGHT",
                "up": "JOY1_UP",
                "down": "JOY1_DOWN",
            }]

        #f.write("input.config=1\n")
        
        f.write("\n[input]\n")
        #f.write("joyport0=none\n")
        #f.write("joyport1=none\n")
        #f.write("input.1.keyboard.0.empty=false\n")
        #f.write("input.1.keyboard.0.disabled=false\n")

        for i, input in enumerate(self.inputs):
            if not input.device:
                continue
            if i == 0:
                key = "joystick_port_1"
            else:
                key = "joystick_port_0"
            if input.type == "amiga_mouse":
                value = "mouse"
            else:
                value = input.device.id;
            print("configure device:", key, value)
            f.write("{0} = {1}\n".format(key, value));

    def find_kickstart(self, bios_patterns):
        print("find_kickstart", bios_patterns)
        for name in os.listdir(self.firmware_dir):
            for bios in bios_patterns:
                if bios in name:
                    return os.path.join(self.firmware_dir, name)
        raise Exception("Could not find Amiga bios/kickstart " +
                repr(bios_patterns))

    def get_game_refresh_rate(self):
        # FIXME: Now assuming that all games are PAL / 50 Hz
        # - make configurable?
        return 50.0

class InputOption(GameHandler.Option):

    def __init__(self, number, value):
        GameHandler.Option.__init__(self)
        self.priority = 0.2 - 0.1 * number
        if value == "joystick":
            self.title = "Joystick"
        elif value == "mouse":
            self.title = "Mouse"
        else:
            self.title = "Nothing"
        self.value = value
        if number == 1:
            self.subtitle = "Joystick Port {0}".format(1)
        else:
            self.subtitle = "Joystick Port {0}".format(0)

    def activate(self):
        print("InputOption.activate")
        return [
            {"title": "Nothing", "subtitle": "", "value": ""},
            {"title": "Joystick", "subtitle": "", "value": "joystick"},
            {"title": "Mouse", "subtitle": "", "value": "mouse"},
        ]


"""
class InputMapper(GameHandler.InputMapper):

    def axis(self, axis, positive):
        axis_start = self.device.buttons
        offset = 1 if positive else 0
        return "input.1.joystick.{0}.button.{1}".format(
                self.device.index, axis_start + axis * 2 + offset)

    def hat(self, hat, direction):
        hat_start = self.device.buttons + self.device.axes * 2
        offset = {
            "left": 0,
            "right": 1,
            "up": 2,
            "down": 3,
        }[direction]
        return "input.1.joystick.{0}.button.{1}".format(
                self.device.index, hat_start + hat * 4 + offset)

    def button(self, button):
        return "input.1.joystick.{0}.button.{1}".format(
                self.device.index, button)

    def key(self, key):
        return "input.1.keyboard.0.button.{0}.{1}".format(
                key.dinput_code, key.dinput_name[4:])
"""