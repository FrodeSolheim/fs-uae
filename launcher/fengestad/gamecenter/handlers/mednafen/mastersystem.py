from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


from fengestad.gamecenter_handlers.mess import MessController
from ._pyapp_plugin import pyapp_plugin, logger, ngettext, _


class MasterSystemController(MessController):

    def configure_media(self):
        self.configure_cartridge()

    def get_romset(self):
        if self.get_game_refresh_rate() == 50:
            return 'smspal'
        else:
            return 'sms'

    def get_input_mapping(self):
        return {
            'P#_START': 'START',
            'P#_JOYSTICK_UP': 'UP',
            'P#_JOYSTICK_DOWN': 'DOWN',
            'P#_JOYSTICK_LEFT': 'LEFT',
            'P#_JOYSTICK_RIGHT': 'RIGHT',
            'P#_BUTTON1': '1',
            'P#_BUTTON2': '2',
        }

    def get_offset_and_scale(self):
        if self.get_romset() == 'smspal':
            return 0.0, 0.0, 1.082, 1.250
        return 0.0, 0.0, 1.082, 1.164



"""
        #path = self.context.game.file
        #self.args.extend(['-s6d1', path])
        #self.args.extend(['-flop1', path])

        #disks_dir = os.path.join(self.context.temp.dir('mess'), 'Disks')
        #os.makedirs(disks_dir)
        #found_disks = DiskUtil.find_disks(self.context.game.file,
        #        script=self.context.game.script)
        #disks = []
        #for p in found_disks:
        #    dest_path = os.path.join(disks_dir, os.path.basename(p))
        #    print("copy", p, "=>", dest_path)
        #    shutil.copy(p, dest_path)
        #    disks.append(dest_path)
        #
        self.args.extend(['-cart',  self.context.game.file])
"""


"""
class MasterSystemController(MednafenController):

    #def is_pal_game(self):
    #    # FIXME
    #    return True

    def get_game_size(self):
        # FIXME
        return (256, 224)
        #return (256, 240)

    def force_aspect_ratio(self):
        # FIXME: Correct?
        return 4/3
        #return None

    def get_system_prefix(self):
        return "sms"

    def get_rom_extensions(self):
        return [".sms"]

    def get_scanlines_setting(self):
        return 33

    def get_special_filter(self):
        return "nn2x"

    def get_extra_graphics_options(self):
        options = []
        #if self.is_pal_game():
        #    options.extend(["-nes.pal", "1"])
        #else:
        #    options.extend(["-nes.pal", "0"])
        return []

    def get_game_refresh_rate(self):
        # can be overriden by subclasses
        #return None
        refresh = MednafenController.get_game_refresh_rate(self)
        if refresh == 50.0:
            # FIXME: Mednafen does not support pal master system ???
            return None


    def get_mednafen_input_config(self):
        return "Sega Master System", [{
                "sms.input.port1.gamepad.fire1":  "1",
                "sms.input.port1.gamepad.fire2":  "2",
                "sms.input.port1.gamepad.up":     "UP",
                "sms.input.port1.gamepad.down":   "DOWN",
                "sms.input.port1.gamepad.left":   "LEFT",
                "sms.input.port1.gamepad.right":  "RIGHT",
                #"nes.input.port1.gamepad.pause":  "PAUSE",
            }, {
                "sms.input.port2.gamepad.fire1":  "1",
                "sms.input.port2.gamepad.fire2":  "2",
                "sms.input.port2.gamepad.up":     "UP",
                "sms.input.port2.gamepad.down":   "DOWN",
                "sms.input.port2.gamepad.left":   "LEFT",
                "sms.input.port2.gamepad.right":  "RIGHT",
                #"nes.input.port1.gamepad.pause":  "PAUSE",
            }]
"""

