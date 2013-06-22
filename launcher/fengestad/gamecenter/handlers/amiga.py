from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
import subprocess
#import pkg_resources
from fengestad import fs
from fengestad.gamecenter.diskutil import DiskUtil
from fengestad.gamecenter.nameutil import NameUtil
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.input.sdlkeycodes import sdl_key_codes
#from fengestad.input.dinputkeycodes import sdlk_to_dik, dinput_key_codes
from .amigaprofiles import AmigaProfiles
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class AmigaController(GameController):

    def init(self):
        # #self.port1_option = InputOption(1,
        # #        self.context.game.config.get("joyport_1", "joystick"))
        # self.port1_option = InputOption(1,
        #         self.context.game.config.get("port1", "joystick"))
        # self.options.append(self.port1_option)
        # #self.port0_option = InputOption(0,
        # #        self.context.game.config.get("joyport_0", "mouse"))
        # self.port0_option = InputOption(0,
        #         self.context.game.config.get("port0", "mouse"))
        # self.options.append(self.port0_option)
        pass

    def init_input(self):
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
        #if fs.windows:
        #    self.input_device_order = "ALPHABETICAL"

    def prepare(self):
        print("AmigaController.prepare")
        self.firmware_dir = self.prepare_firmware("Amiga Firmware")
        #self.save_disk_dir = self.prepare_dir("amiga_save_disk",
        #        [os.path.join(pyapp_plugin.path, "resources")])

    def configure(self):
        print("AmigaController.configure")
        temp_dir = self.context.temp.dir("amiga-config")
        config_file = os.path.join(temp_dir, "amiga-config.cfg")
        with open(config_file, "wb") as f:
            self._configure_emulator(f)
        self.args.extend(["--config", config_file])

    def run(self):
        cwd = os.path.join(self.context.temp.dir("uae"))
        emulator = "fs-uae"
        return self.run_emulator(emulator, cwd=cwd)

    def cleanup(self):
        self.changes.update(self.context.game.state_dir)

    def get_supported_filters(self):
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
        return supported

    def _configure_emulator(self, f):
        if "AGA" in self.context.game.config["name"]:
            machine = "amiga 1200"
        elif "ECS" in self.context.game.config["name"]:
            machine = "amiga 600"
        else:
            machine = self.context.game.config.machine or "amiga 500"
        machine = machine.lower()
        if machine == "amiga 500":
            bios_patterns, config = AmigaProfiles.amiga_500
            amiga_model_id, amiga_model_name = "A500", "Amiga 500"
        elif machine == "amiga 600":
            bios_patterns, config = AmigaProfiles.amiga_600
            amiga_model_id, amiga_model_name = "A600", "Amiga 600"
        elif machine == "amiga 1200":
            bios_patterns, config = AmigaProfiles.amiga_1200
            amiga_model_id, amiga_model_name = "A1200", "Amiga 1200"
        else:
            raise Exception("Unknown Amiga machine " + repr(machine))
        
        f.write("[application]\n")
        f.write("title = {0}\n".format(amiga_model_name))
        f.write("sub_title = {0}\n".format(self.context.game.name))
        
        f.write("[amiga]\n")
        f.write("model = {0}\n".format(amiga_model_id))
        
        if self.context.game.config.chip_ram:
            f.write("chip_ram = {0}\n".format(
                    self.context.game.config.chip_ram))
        if self.context.game.config.slow_ram:
            f.write("slow_ram = {0}\n".format(
                    self.context.game.config.slow_ram))
        f.write("kickstart_file = {path}\n".format(
                path=self.find_kickstart(bios_patterns)))
       
        # media options
        # FIXME: Check if save states require that the disks are located
        # at the exact same path every time...
        #working_dir = os.path.join(self.context.temp.dir("uae"), "Disks")
        #os.makedirs(working_dir)
        working_dir = self.context.temp.dir("uae")
        #state_dir = os.path.join(self.context.temp.dir("uae"), "state")
        
        # FIMXE: state path
        
        #f.write("unix.floppy_path={0}\n".format(working_dir + os.sep))
        #state_file = os.path.join(working_dir, "Save.uss")
        #f.write("statefile={path}\n".format(path=state_file))

        found_disks = DiskUtil.find_disks(self.context.game.file,
                script=self.context.game.script)
        disks = []
        for p in found_disks:
            dest_path = os.path.join(working_dir, os.path.basename(p))
            print("copy", p, "=>", dest_path)
            shutil.copy(p, dest_path)
            disks.append(dest_path)
            
            name, ext = os.path.splitext(dest_path)
            ext = ext.lower()
            use_save_overlay_for = [".ipf"]
            # FIXME: good idea to use overlays for ADF files too, or just
            # use for ipf files?
            use_save_overlay_for.append(".adf")
            if ext in use_save_overlay_for:
                overlay_save_file = name + ".sav" 
                stream = resources.resource_stream("amiga_blank_disk.adf")
                with open(overlay_save_file, "wb") as save_f:
                    save_f.write(stream.read())

        # Most games require no save disk, but we copy in one always, so one
        # is available. If it is not used, the change handler will not copy
        # it to the state dir anyway.
       
        if self.context.game.config.get("bootfloppies", None):
            numbered_disks = {}
            if self.context.game.script:
                for disk in disks:
                    renamed = self.context.game.script.rename_file(disk)
                    number = NameUtil.find_number(renamed)
                    numbered_disks[number] = disk
            else:
                for i, disk in enumerate(disks):
                    numbered_disks[number] = disk
            sorted_disks = []
            boot_floppies = self.context.game.config.get("bootfloppies")
            for num in boot_floppies.split(","):
                num = int(num)
                sorted_disks.append(numbered_disks[num])
        else:
            sorted_disks = disks[:]

        #save_disk = self.resource_file("Save Disk.adf")
        #save_disk = os.path.join(self.save_disk_dir, "Save Disk.adf")
        #shutil.copy(save_disk, os.path.join(working_dir, "Save Disk.adf"))
        stream = resources.resource_stream("amiga_save_disk.adf")
        with open(os.path.join(working_dir, "Save Disk.adf"), "wb") as save_f:
            save_f.write(stream.read())

        disks.append(os.path.join(working_dir, "Save Disk.adf"))
        sorted_disks.append(os.path.join(working_dir, "Save Disk.adf"))


        self.changes = ChangeHandler(working_dir)
        self.changes.init(self.context.game.state_dir)

        print("GAME CONFIGURATION VALUES ARE", self.context.game.config)
        num_floppy_drives = int(self.context.game.config.get(
                "floppy_drives", "4"))
        #sorted_disks = self.sort_disks(disks)
        # FIXME: CAN REPLACE sorted_disks WITH disks

        f.write("[media]\n")

        # do not insert last save disk by default
        inserted_disks = sorted_disks[:min(num_floppy_drives, len(sorted_disks) - 1)]
        #print(inserted_disks)
        num_floppy_drives = min(num_floppy_drives, len(inserted_disks))
        #for i, path in enumerate(inserted_disks): #range(len(sf)):
        for i in range(num_floppy_drives):
            if i < len(inserted_disks):
                path = inserted_disks[i]
                #if sf[i] == -1:
                #    # Save disk is always last in floppy list
                #    floppyindex = len(disks) - 1
                #else:
                #    floppyindex = sf[i] - 1
                #try:
                #    path = disks[floppyindex]
                #except IndexError:
                #    raise IndexError("Floppy %d not found" % floppyindex)
                path = fs.encode_path(path)
                #f.write("floppy{0}={1}\n".format(i, os.path.join("Disks", 
                #        os.path.basename(path))))
                f.write("floppy_drive_{0} = {1}\n".format(i,
                        os.path.basename(path)))
            else:
                f.write("floppy_drive_{0} = {1}\n".format(i, ""))
            #f.write("df{0}type = 0\n".format(i))
            #f.write("df{0}sound = 1\n".format(i))
            #if fs.windows:
            #    f.write("floppy{0}sound=1\n".format(i))
            #else:
            #    f.write("floppy{0}sound=-1\n".format(i))
                #f.write("floppy%dsoundext=default.wav\n" % (i,))
                #f.write("floppy%dsoundext=%s\n" % (i, drive_sound_dir))
        #for i in range(len(inserted_disks), 4):
        
        #for i in range(num_floppy_drives, 4):
        #    f.write("df{0}type = -1\n".format(i))

        # FIXME: or is this number of disks in a floppy list?
        #f.write("nr_disks={0}\n".format(num_floppy_drives))
        f.write("floppy_drive_count = {0}\n".format(num_floppy_drives))

        floppy_speed = 100
        floppy_volume = 80        
        f.write("floppy_drive_speed = {0}\n".format(floppy_speed))
        f.write("floppy_drive_volume ={0}\n".format(floppy_volume))
        
        #print(disks)
        for i, path in enumerate(disks):
            n = i
            if i == len(disks) - 1:
                # last disk is always save disk
                n = 19
            #f.write("diskimage{0}={1}\n".format(n, os.path.join("Disks",
            #        os.path.basename(path))))
            f.write("floppy_image_{0} = {1}\n".format(n,
                    os.path.basename(path)))

        #sound_latency = fs.("UAE/SoundLatency", None)
        #if sound_latency:
        #    # convert to microseconds
        #    sound_latency = int(sound_latency) * 1000
        #    f.write("sound_latency={0}\n".format(sound_latency))

        self.set_input_options(f)
        #if fs.windows:
        #    self.set_graphics_options_windows(f)
        #else:
        self.set_graphics_options(f)

    def set_input_options(self, f):
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
                
            #if input.device.is_joystick():
            #    f.write("input.1.joystick.{0}.empty=false\n".format(
            #            input.device.index))
            #    f.write("input.1.joystick.{0}.disabled=false\n".format(
            #            input.device.index))
            #mapper = InputMapper(input, input_mapping[i])
            #for key, value in mapper.iteritems():
            #    f.write("{value}={key}.0\n".format(
            #            key=key, value=value))

        #if self.inputs[1].type == "amiga_mouse":
        #    f.write("input.1.mouse.0.empty=false\n")
        #    f.write("input.1.mouse.0.disabled=false\n")
        #    f.write("input.1.mouse.0.axis.0=MOUSE1_HORIZ\n")
        #    f.write("input.1.mouse.0.axis.1=MOUSE1_VERT\n")
        #    f.write("input.1.mouse.0.button.0=JOY1_FIRE_BUTTON\n")
        #    f.write("input.1.mouse.0.button.1=JOY1_2ND_BUTTON\n")
        #    f.write("input.1.mouse.0.button.2=JOY1_3RD_BUTTON\n")

    def find_kickstart(self, bios_patterns):
        print("find_kickstart", bios_patterns)
        for name in os.listdir(self.firmware_dir):
            for bios in bios_patterns:
                if bios in name:
                    return os.path.join(self.firmware_dir, name)
        raise Exception("Could not find Amiga bios/kickstart " +
                repr(bios_patterns))

    def set_graphics_options(self, f):
        
        f.write("\n[video]\n")
        f.write("frame_rate = {0}\n".format(
                int(self.get_game_refresh_rate())))
        
        
        #f.write("\n[display]\n")        
        # FIXME: for PAL games only
        #f.write("fps = {0}\n".format(int(self.get_game_refresh_rate())))
        
        # FIXME ...
        install_mode = False
        
        #if self.get_option("smooth"):
        #    f.write("gfx_filter_bilinear=true\n")
        
        if self.context.game.config.viewport:
            viewport = self.context.game.config.viewport
            if "=>" not in viewport:
                viewport = "* * * * => " + viewport
            f.write("viewport = {0}\n".format(str(viewport)))

        if self.get_option("fullscreen"):
            # FIXME: CONFIG VALUE
            #self.args.append("--fullscreen")
            f.write("fullscreen = 1\n")

        if self.configure_vsync():
            self.args.append("--vsync")
        
        if self.get_option("fsaa"):
            f.write("fsaa = {0}\n".format(str(self.get_option("fsaa"))))
            
        #    f.write("gfx_vsync=true\n")
        #else:
        #   f.write("gfx_vsync=false\n")

    def get_game_refresh_rate(self):
        # FIXME: Now assuming that all games are PAL / 50 Hz
        # - make configurable?
        return 50.0


class InputOption(GameController.Option):

    def __init__(self, number, value):
        GameController.Option.__init__(self)
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
class InputMapper(GameController.InputMapper):

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