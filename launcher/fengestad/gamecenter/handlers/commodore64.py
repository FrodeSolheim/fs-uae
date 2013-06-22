from __future__ import with_statement
from __future__ import absolute_import
from __future__ import division


import os
import math
import tempfile
import subprocess
import shutil
import wx
from fengestad import fs
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.refreshratetool import RefreshRateTool
from fengestad.input.sdlkeycodes import sdl_key_codes
from fengestad.input.dinputkeycodes import dinput_key_codes
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class Commodore64Controller(GameController):

    def init(self):
        self.port1_option = InputOption(1,
                self.context.game.config.get('port1', 'joystick'))
        self.options.append(self.port1_option)
        self.port0_option = InputOption(0,
                self.context.game.config.get('port0', 'mouse'))
        self.options.append(self.port0_option)

    def init_input(self):
        mapping = {
            'joystick': ('commodore64', 'Joystick'),
            'mouse': ('commodore64_mouse', 'Mouse'),
            '': ('', '')
        }
        self.inputs = [
            self.create_input(name='Joystick Port 1',
                    type=mapping[self.port1_option.value][0],
                    description=mapping[self.port1_option.value][1]),
            self.create_input(name='Joystick Port 0',
                    type=mapping[self.port0_option.value][0],
                    description=mapping[self.port0_option.value][1])
        ]
        if fs.windows:
            self.input_device_order = 'DINPUT8'

    def configure(self):
        with open(self.get_vice_cfg_path(), 'wb') as f:
            f.write(default_config)
            self._configure_emulator(f)
            self.configure_input(f)

    def run(self):
        #cwd = os.path.join(self.context.temp.dir('vice'))
        #return self.run_emulator('x64', cwd=cwd)
        emulator = "x64sc"
        return self.run_emulator(emulator)

    #def create_emulator_process(self, env):
    #    plugin_id = 'no.fengestad.emulator.vice'
    #    plugin = pyapp.plug.get_plugin(plugin_id)
    #    #self.args.append("+confirmexit")
    #    if fs.windows:
    #        temp_dir = self.context.temp.dir('vice')
    #        GameCenterUtil.copy_folder_tree(plugin.get_bin_dir(), temp_dir)
    #        args = self.args[:]
    #        args.insert(0, os.path.join(temp_dir, 'x64.exe'))
    #        os.chdir(temp_dir)
    #        return subprocess.Popen(args, env=env, close_fds=True)
    #    print(self.args)
    #    from pprint import pprint
    #    pprint(env)
    #    return plugin.x64(self.args, env=env, close_fds=True)

    def _configure_emulator(self, f):
#            f.write("[C64]\n")

        #if fs.linux:
        #    self.args.extend(["-sounddev", "esd"])
        #    self.args.extend(["-soundbufsize", "25"])
        #    self.args.extend(["-soundsync", "2"])
        #    self.args.extend(["-soundrate", "44100"])
        #self.args.extend(["-residsamp", "2"])

        f.write('SDLCustomWidth={w}\n'.format(
                w=self.context.system.screen_width))
        f.write('SDLCustomHeight={h}\n'.format(
                h=self.context.system.screen_height))
        if self.get_option("fullscreen"):
            f.write('VICIIFullscreen=1\n')
            f.write('FullscreenEnabled=1\n')
            if fs.linux:
                self.args.append("-VICIIhwscale")
            #    hwscoords = self.set_hwscale_coords()
            else:
                pass
            #    #if self.options["zoom"]:
            #    #    logger.warning("zoom is not supported on this platform")
            #    #if self.options["stretch"]:
            #    #    logger.warning("stretch is not supported on this platform")
            #    #if self.options["border"]:
            #    #    logger.warning("border is not supported on this platform")
            if fs.macosx:
                #logger.warning("Fullscreen not supported on Mac OS X")
                pass
            else:
                self.args.append("-fullscreen")
        #if fs.linux:
        #    self.args.extend(["-VICIIXRANDRfullmode", "0"])
        #self.args.append("-VICIIdsize")

        filter_data = self.configure_filter()
        """
        effect = 'bilinear'
        filter_mapping = {
            'auto': 'pal',
            'pal': 'pal',
            'nearest': 'nearest',
        }
        for filter in self.context.config.filters:
            try:
                effect = filter_mapping[filter]
            except KeyError:
                continue
            break
        if effect == 'pal':
            f.write('PALEmulation=1\n')
            # scanlines does not look good since they are applied
            # before fullscreen scaling (1000 = no shade)
            f.write('PALScanLineShade=1000\n')
            f.write('VICIIDoubleSize=1\n')
        elif effect == 'nearest':
            f.write('VICIIDoubleSize=1\n')
        else:
            f.write('VICIIDoubleSize=0\n')
        """

        if self.configure_vsync():
            pass

        print "set_media_options"
        media_list = self.create_media_list()
        print "sort media list"
        # FIXME: SORT ON name (lowercase) ONLY, not whole path, because
        # some files may have moved to another dir (temp dir)
        media_list = sorted(media_list)
        print media_list
        if media_list[0].lower()[-4:] == '.crt':
            f.write('CartridgeFile="{path}"\n'.format(path=media_list[0]))
            f.write('CartridgeType={type}\n'.format(type=0))
            f.write('CartridgeMode={mode}\n'.format(mode=0))
            f.write('CartridgeReset={reset}\n'.format(reset=1))
        #    #self.args.extend(["-autostart", media_list[0]])
        else:
            #f.write('AutostartPrgMode=2\n') # disk image
            #f.write('AutostartPrgDiskImage="{path}"\n'.format(
            #        path=media_list[0]))
            self.args.extend(["-autostart", media_list[0]])
        # FIXME: Floppies?
        print(media_list)
        if media_list[0].lower()[-4:] == ".d64":
            f = self.context.temp.file('fliplist')
            f.write("# Vice fliplist file\n\n");
            f.write("UNIT 8\n");
            print("FLIP LIST:")
            # Make sure first disk is added to the end of the fliplist
            for floppy in (media_list[1:] + [media_list[:1]]):
                print("%s\n" % (floppy,))
                print
                f.write("%s\n" % (floppy,))
            f.close()
            self.args.extend(["-flipname", self.context.temp.file(
                    'fliplist')])
        media_dir = os.path.dirname(media_list[0])
        print(media_dir)
        f.write('InitialDefaultDir="{dir}"\n'.format(dir=media_dir))
        f.write('InitialTapeDir="{dir}"\n'.format(dir=media_dir))
        f.write('InitialCartDir="{dir}"\n'.format(dir=media_dir))
        f.write('InitialDiskDir="{dir}"\n'.format(dir=media_dir))
        f.write('InitialAutostartDir="{dir}"'.format(dir=media_dir))

    def configure_input(self, f):
        # FIXME: From amiga - replace
        input_mapping = [{
            '1': 'JOY2_FIRE_BUTTON',
            '2': 'JOY2_2ND_BUTTON',
            '3': 'JOY2_3RD_BUTTON',
            'left': 'JOY2_LEFT',
            'right': 'JOY2_RIGHT',
            'up': 'JOY2_UP',
            'down': 'JOY2_DOWN',
        }, {
            '1': 'JOY1_FIRE_BUTTON',
            '2': 'JOY1_2ND_BUTTON',
            '3': 'JOY1_3RD_BUTTON',
            'left': 'JOY1_LEFT',
            'right': 'JOY1_RIGHT',
            'up': 'JOY1_UP',
            'down': 'JOY1_DOWN',
        }]

        for i, input in enumerate(self.inputs):
            if not input.device:
                continue
            mapper = InputMapper(input, input_mapping[i])
            for key, value in mapper.iteritems():
                f.write('{value}={key}.0\n'.format(
                        key=key, value=value))        

    def __xxx__(self,f ):
        #inputs = ("MOUSE", "JOYSTICK")
        inputs = ("JOYSTICK", "JOYSTICK")

        #try:
        #    inputs = self.version_info["controllers"].split(",")
        #    inputs = (inputs[0].strip().upper(), inputs[1].strip().upper())
        #except KeyError:
        #    inputs = ("MOUSE", "JOYSTICK")
        #inputs = ("JOYSTICK", "JOYSTICK")
        keyset = 0
        joydevicecount = list(inputs).count('JOYSTICK')
        joydevices = []
        if fs.windows:
            device_order = 'DINPUT8'
        else:
            device_order = 'SDL'
        devices = self.context.input.get_devices(joydevicecount,
                device_order=device_order)
        def calc_button(value):
            if len(value) == 4 and value[:2] == 'JA':
                return 0
            elif len(value) == 4 and value[:2] == 'JH':
                return 0
            elif len(value) == 4 and value[:2] == 'JB':
                return 0
            else:
                if fs.windows:
                    return dinput_key_codes[value]
                else:
                    return sdl_key_codes[value]       
        for i, device in enumerate(devices):
            device_config = device.get_config()
            print("C64 JOYSTICK", device.get_name())
            if device.get_type() == 'TYPE_KEYBOARD':
                keyset += 1
                f.write("KeySet%dNorth=%d\n" % (keyset,
                        calc_button(device_config["UP"])))
                f.write("KeySet%dSouth=%d\n" % (keyset,
                        calc_button(device_config["DOWN"])))
                f.write("KeySet%dWest=%d\n" % (keyset,
                        calc_button(device_config["LEFT"])))
                f.write("KeySet%dEast=%d\n" % (keyset,
                        calc_button(device_config["RIGHT"])))
                f.write("KeySet%dFire=%d\n" % (keyset,
                        calc_button(device_config["FIRE"])))
                joydevices.append(1 + keyset)
            else:
                joydevices.append(4 + device.index)
        if keyset > 0:
            f.write("KeySetEnable=1\n")
        j = joydevices[0]
        if inputs[1] == "JOYSTICK":
            f.write("JoyDevice2=%d\n" % (j,))
            if joydevicecount > 1:
                j = joydevices[1]
        if inputs[0] == "JOYSTICK":
            f.write("JoyDevice1=%d\n" % (j,))

    def get_vice_cfg_path(self):
        if fs.windows:
            temp_dir = self.context.temp.dir('vice')
            return os.path.join(temp_dir, "vice.ini")
        else:
            if not os.path.exists(os.path.join(pyapp.user.home_dir(),
                    '.vice')):
                os.makedirs(os.path.join(pyapp.user.home_dir(),
                    '.vice'))
            return os.path.join(pyapp.user.home_dir(), '.vice',
                    'sdl-vicerc')

#    def set_hwscale_coords(self):
#        sourcex, sourcey = 0, 0
#        sourcew, sourceh = 768, 544
#        screenx, screeny = 0, 0
#        screenw, screenh = self.get_fullscreen_size()
#        if self.options["zoom"]:
#            sourcex, sourcey = 64, 70
#            sourcew, sourceh = 640, 400
#            if self.options["border"]:
#                border = self.get_output_border()
#                screenx += border[0]
#                screeny += border[1]
#                screenw -= border[0] + border[2]
#                screenh -= border[1] + border[3]
#
#        if self.options["stretch"]:
#            targetx = screenx
#            targety = screenx
#            targetw = screenw
#            targeth = screenh
#        else:
#            scale = scale = min(screenw / sourcew, screenh / sourceh)
#            targetw = sourcew * scale
#            targeth = sourceh * scale
#            targetx = screenx + (screenw - targetw) / 2
#            targety = screeny + (screenh - targeth) / 2
#        print "source rect", (sourcex, sourcey, sourcew, sourceh)
#        print "target rect", (targetx, targety, targetw, targeth)
#
#        # source rect is
#        screenw, screenh = self.get_fullscreen_size()
#        scalex = (targetw / sourcew) / (screenw / 768)
#        scaley = (targeth / sourceh) / (screenh / 544)
#        # fixme: consider targetx and targety also. only targetw and
#        # targeth are (indirectly) used here.
#        transx = (sourcex + sourcew / 2 - 768 / 2) / (768 / 2) * scalex
#        transy = (sourcey + sourceh / 2 - 544 / 2) / (544 / 2) * scaley
#        print scalex, scaley
#        print transx, transy
#        self.args.extend(["-hwscaletx", str(int(transx * 10000))])
#        self.args.extend(["-hwscalety", str(int(transy * 10000))])
#        self.args.extend(["-hwscalesx", str(int(scalex * 10000))])
#        self.args.extend(["-hwscalesy", str(int(scaley * 10000))])

    # FIXME: COMMON WITH AMIGA ALSO..
    def extract_match_name(self, name):
        name = os.path.normcase(name)
        pos_list = []
        pos_list.append(name.find('('))
        pos_list.append(name.find('['))
        pos_list.append(name.rfind('.'))
        pos_list.append(len(name))
        for pos in sorted(pos_list):
            if pos > -1:
                return name[:pos].strip().lower()

    def create_media_list(self):
        in_list = [self.context.game.file]
        media_dir = os.path.dirname(self.context.game.file)
        base_match = self.extract_match_name(os.path.basename(
                self.context.game.file))
        for name in os.listdir(media_dir):
            match = self.extract_match_name(name)
            if base_match == match:
                in_list.append(os.path.join(media_dir, name))
        out_list = []
        for medium in in_list:
            if medium.endswith('.nib'):
                temp_dir = self.context.temp.dir('media')
                temp_file = os.path.join(temp_dir, os.path.basename(
                        medium).replace('.nib', '.g64'))
                p = pyapp.plugins.get_plugin('no.fengestad.nibtools').nibconv(
                        [medium, temp_file])
                p.wait()
                out_list.append(temp_file)
            else:
                out_list.append(medium)
        return out_list
        #data_dir_name = self.version_info["datadirectory"]
        #data_dir = os.path.join(self.game_info.game_dir, "Gamedata",
        #        data_dir_name)
        #floppies = []
        #for item in os.listdir(data_dir):
        #    itemlower = item.lower()
        #    name, ext = os.path.splitext(itemlower)
        #    if ext in ('.d64',):
        #        floppies.append((itemlower, os.path.join(data_dir, item)))
        #    if ext in ('.t64',): # Tape image
        #        floppies.append((itemlower, os.path.join(data_dir, item)))
        #    if ext in ('.tap',): # Tape image
        #        floppies.append((itemlower, os.path.join(data_dir, item)))
        #floppies.sort()
        #return [x[1] for x in floppies]



default_config = """[C64]
ConfirmOnExit=0
KeepAspectRatio=1
SDLGLAspectMode=1
VICIIHwScale=1
HwScalePossible=1
VICIIDoubleScan=1
VICIISDLFullscreenMode=1
AutostartWarp=0
Drive8Type=1541
"""


class InputOption(GameController.Option):

    def __init__(self, number, value):
        self.priority = 0.2 - 0.1 * number
        if value == 'joystick':
            self.title = "Joystick"
        elif value == 'mouse':
            self.title = "Mouse"
        else:
            self.title = "Nothing"
        self.value = value
        if number == 1:
            self.subtitle = "Joystick Port {0}".format(1)
        else:
            self.subtitle = "Joystick Port {0}".format(0)


class InputMapper(GameController.InputMapper):

    def axis(self, axis, positive):
        axis_start = self.device.buttons
        offset = 1 if positive else 0
        return 'input.1.joystick.{0}.button.{1}'.format(
                self.device.index, axis_start + axis * 2 + offset)

    def hat(self, hat, direction):
        hat_start = self.device.buttons + self.device.axes * 2
        offset = {
            'left': 0,
            'right': 1,
            'up': 2,
            'down': 3,
        }[direction]
        return 'input.1.joystick.{0}.button.{1}'.format(
                self.device.index, hat_start + hat * 4 + offset)

    def button(self, button):
        return 'input.1.joystick.{0}.button.{1}'.format(
                self.device.index, button)

    def key(self, key):
        return 'input.1.keyboard.0.button.{0}.{1}'.format(
                key.dinput_code, key.dinput_name[4:])
