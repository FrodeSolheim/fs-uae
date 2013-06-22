from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
import wx
from fengestad import fs
from fengestad.gamecenter.changehandler import ChangeHandler
from fengestad.gamecenter.gamecenterutil import GameCenterUtil
from fengestad.gamecenter.gamecontroller import GameController
from fengestad.gamecenter.resources import resources, logger, ngettext, _


class PlayStationController(GameController):

    def init_input(self):
        self.inputs = []
        for i in range(2):
            self.inputs.append(self.create_input(
                    name='Controller {0}'.format(i + 1),
                    type='playstation',
                    description='Controller'))
        if fs.windows:
            self.input_device_order = 'DINPUT8'

    def prepare(self):
        self.prepare_cdroms()
        self.firmware_dir = self.prepare_firmware('PlayStation Firmware')

    def configure(self):
        self.pcsx_ini = []
        self.dfxvideo_cfg = []
        self.dfsound_cfg = []
        self.dfbinimage_cfg = []

        #self.set_sound_options()
        #self.set_graphics_options()
        #self.set_input_options()
        self.set_media_options()
        self.set_other_options()
        if fs.windows:
            self.configure_input_windows()
            self.configure_sound_windows()
            self.configure_graphics_windows()

    def run(self):
        plugin = pyapp.plug.get_plugin('no.fengestad.emulator.pcsx')
        #if fs.windows:
        #    data_path = os.path.join(pyapp.fs.data_dir_user_app(), "zsnes")
        #    # Dir should have been created by config file writing
        #    for item in os.listdir(plugin.program_dir_path):
        #        if item[0] == ".":
        #            continue
        #        shutil.copyfile(os.path.join(plugin.program_dir_path, item),
        #                os.path.join(data_path, item))
        #    procargs = self.args[:]
        #    procargs.insert(0, os.path.join(data_path, "zsnesw.exe"))
        #    os.chdir(data_path)
        #   #with open(self.zinput_cfg_path(), "rb") as cfg:
        #    #    print cfg.read()
        #    #print "stop"
        #    #import sys
        #    #sys.exit(1)
        #    return subprocess.Popen(procargs)
        args = self.args[:]
        args.extend(['-nogui'])
        args.extend(['-cdfile', self.get_cd_image()])
        return plugin.pcsx(args, env=self.env)


        #self._fake_home = self.get_temp_dir_path('fake_home')

        """
        pcsx_home = pyapp.user.home_dir()
        #p = os.path.join(self._fake_home, '.pcsx', 'pcsx.ini')
        p = os.path.join(pcsx_home, '.pcsx', 'pcsx.ini')
        if not os.path.exists(os.path.dirname(p)):
            os.makedirs(os.path.dirname(p))
        with open(p, 'wb') as f:
            f.write('\n'.join(self.pcsx_ini).encode('UTF-8'))

        p = os.path.join(pcsx_home, '.pcsx', 'plugins', 'dfxvideo.cfg')
        if not os.path.exists(os.path.dirname(p)):
            os.makedirs(os.path.dirname(p))
        with open(p, 'wb') as f:
            f.write('\n'.join(self.dfxvideo_cfg).encode('UTF-8'))

        p = os.path.join(pcsx_home, '.pcsx', 'plugins', 'dfsound.cfg')
        if not os.path.exists(os.path.dirname(p)):
            os.makedirs(os.path.dirname(p))
        with open(p, 'wb') as f:
            f.write('\n'.join(self.dfsound_cfg).encode('UTF-8'))

        p = os.path.join(pcsx_home, '.pcsx', 'plugins', 'dfbinimage.cfg')
        if not os.path.exists(os.path.dirname(p)):
            os.makedirs(os.path.dirname(p))
        with open(p, 'wb') as f:
            f.write('\n'.join(self.dfbinimage_cfg).encode('UTF-8'))
        """


        #self.run_emulator()

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

    def sort_cdroms(self, cdroms):
        return sorted(cdroms)

    def prepare_cdroms(self):
        cdroms = []
        media_dir = os.path.dirname(self.context.game.file)
        base_match = self.extract_match_name(os.path.basename(
                self.context.game.file))
        for name in os.listdir(media_dir):
            match = self.extract_match_name(name)
            if base_match == match:
                cdroms.append(os.path.join(media_dir, name))
                #cdroms.append(name)
        self.cdroms = cdroms
        #return cdroms

    #def post_run_emulator(self, process):
    #    if fs.windows:
    #        self.autostart_windows()

    """
    @pyapp.thread.runs_in_new
    def autostart_windows(self):
        ID_FILE_RUNCDBIOS = 40026
        start_time = time.time()
        window = [None]
        def window_callback(hwnd, extra):
            try:
                classname = win32gui.GetClassName(hwnd)
                if classname == 'PCSX Main':
                    window[0] = hwnd
                #if 'PCSX' in win32gui.GetWindowText(hwnd):
                #    print hwnd
                #    print repr(classname)
                #    print win32gui.GetWindowText(hwnd),
                    #print win32gui.GetWindowModuleFileName(hwnd),
                    #print win32gui.GetWindowClass(hwnd)
                    #print win32gui.GetWindowThreadProcessId(hwnd)
                    #p = ctypes.wintypes.DWORD()
                    #t = ctypes.windll.user32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
                    #print t,
            except Exception, e:
                print e

        while True:
            if time.time() - start_time > 5:
                print "could not find pcsx window, aborting"
                return
            print "Start enumeration"
            win32gui.EnumWindows(window_callback, None)
            if window[0]:
                print "Sending WM_COMMAND/ID_FILE_RUNCDBIOS message"
                win32gui.PostMessage(window[0], win32con.WM_COMMAND,
                        ID_FILE_RUNCDBIOS, 0)
                break
            time.sleep(0.01)
    """

    def configure_input_windows(self):
        input_mapping = {
            'select': SSSPSX_SELECT,
            'start': SSSPSX_START,
            'up': SSSPSX_UP,
            'right': SSSPSX_RIGHT,
            'down': SSSPSX_DOWN,
            'left': SSSPSX_LEFT,
            'r1': SSSPSX_R1,
            'r2': SSSPSX_R2,
            'l1': SSSPSX_L1,
            'l2': SSSPSX_L2,
            'triangle': SSSPSX_TRIANGLE,
            'circle': SSSPSX_CIRCLE,
            'cross': SSSPSX_CROSS,
            'square': SSSPSX_SQUARE,
            'analog': SSSPSX_ANALOG,
        }
        bytes = [0 for x in range(0xaa)]
        for i, input in enumerate(self.inputs):
            assert i < 2
            if not input.device:
                continue
            # config is stored as 170 (0xaa) bytes in a REG_BINARY field
            mapper = SSSPSXInputMapper(input, input_mapping)
            for key, value in mapper.iteritems():
                #print("---->", key, value)
                #if isinstance(key, tuple):
                #    key, index = key
                #else:
                #    index = 0
                #config.setdefault(key, {})[index] = value
                #print(key, " -- ", value)
                if i == 1:
                    offset = 0x54
                else:
                    offset = 0x00
                bytes[offset + key * 2] = value[0]
                bytes[offset + key * 2 + 1] = value[1]
        reg_key = 'HKEY_CURRENT_USER\\Software\\Vision Thing\\' \
                'PSEmu Pro\\PAD\\PadSSSPSX'
        RegistryHelper.delete_key(reg_key)
        RegistryHelper.create_key(reg_key)
        RegistryHelper.set_binary(reg_key, 'cfg', bytes)

    def set_sound_options(self):
        if fs.windows:
            return self.set_sound_options_windows()
        self.dfsound_cfg.append('Volume = 1')
        self.dfsound_cfg.append('UseInterpolation = 3')
        self.dfsound_cfg.append('UseXA = 1')
        self.dfsound_cfg.append('XAPitch = 1')
        self.dfsound_cfg.append('HighCompMode = 1')
        self.dfsound_cfg.append('SPUIRQWait = 0')
        self.dfsound_cfg.append('DisStereo = 0')
        self.dfsound_cfg.append('UseReverb = 0')

    def configure_sound_windows(self):
        reg_key = 'HKEY_CURRENT_USER\\Software\\Vision Thing\\' \
                'PSEmu Pro\\SPU\\PeopsSound'
        RegistryHelper.delete_key(reg_key)
        RegistryHelper.create_key(reg_key)
        RegistryHelper.set_integer(reg_key, 'UseXA', 1)
        RegistryHelper.set_integer(reg_key, 'UseTimer', 1)
        RegistryHelper.set_integer(reg_key, 'XAPitch', 1)
        RegistryHelper.set_integer(reg_key, 'SPUIRQWait', 0)
        RegistryHelper.set_integer(reg_key, 'UseInterpolation', 3)

    def set_graphics_options(self):
        if fs.windows:
            return self.set_graphics_options_windows()
        self.dfxvideo_cfg.append('') # Must begin with blank line
        self.dfxvideo_cfg.append('UseFrameLimit = 1')
        #self.dfxvideo_cfg.append('')
        self.dfxvideo_cfg.append('Maintain43 = 1')
        #self.dfxvideo_cfg.append('')
        self.dfxvideo_cfg.append('FPSDetection = 2') # ???
        #self.dfxvideo_cfg.append('')
        if self.options['fullscreen']:
            width = self.context.system.screen_width
            height = self.context.system.screen_height
        else:
            width, height = 1024, 768
        self.dfxvideo_cfg.append('ResX = %d' % width)
        #self.dfxvideo_cfg.append('')
        self.dfxvideo_cfg.append('ResY = %d' % height)
        #self.dfxvideo_cfg.append('')
        if self.options['fullscreen']:
            self.dfxvideo_cfg.append('FullScreen = 1')
            #self.dfxvideo_cfg.append('')
        else:
            self.dfxvideo_cfg.append('FullScreen = 0')
            #self.dfxvideo_cfg.append('')

    def configure_graphics_windows(self):
        reg_key = 'HKEY_CURRENT_USER\\Software\\Vision Thing\\' \
                'PSEmu Pro\\GPU\\PeteSoft'
        RegistryHelper.delete_key(reg_key)
        RegistryHelper.create_key(reg_key)
        RegistryHelper.set_integer(reg_key, 'ColDepth', 32)
        RegistryHelper.set_integer(reg_key, 'UseDither', 2)
        RegistryHelper.set_integer(reg_key, 'UseFrameLimit', 1)
        RegistryHelper.set_integer(reg_key, 'FrameLimit', 2)
        if self.get_option('stretch'):
            RegistryHelper.set_integer(reg_key, 'UseNoStrechBlt', 0)
        else:
            RegistryHelper.set_integer(reg_key, 'UseNoStrechBlt', 2)
        width = self.context.system.screen_width
        height = self.context.system.screen_height
        RegistryHelper.set_integer(reg_key, 'ResX', width)
        RegistryHelper.set_integer(reg_key, 'ResY', height)
        width, height = 1024, 768
        RegistryHelper.set_integer(reg_key, 'Winsize',
                height * 65536 + width)
        if self.get_option('fullscreen'):
            RegistryHelper.set_integer(reg_key, 'WindowMode', 0)
        else:
            RegistryHelper.set_integer(reg_key, 'WindowMode', 1)
        if self.configure_vsync:
            RegistryHelper.set_integer(reg_key, 'WaitVSYNC', 1)
        else:
            RegistryHelper.set_integer(reg_key, 'WaitVSYNC', 0)

    def set_input_options(self):
        pass

    def set_media_options(self):
        if fs.windows:
            return self.set_media_options_windows()
        cd_image = self.get_cd_image()
        self.dfbinimage_cfg.append('[DFBinImage]')
        self.dfbinimage_cfg.append('lastrun=' + cd_image)
        self.dfbinimage_cfg.append('autorun=' + cd_image)
        #self.dfbinimage_cfg.append('cachemode=')
        #self.dfbinimage_cfg.append('cachesize=10')
        self.dfbinimage_cfg.append('repeat=playOne')
        #self.dfbinimage_cfg.append('subenable=')
        self.dfbinimage_cfg.append('volume=50,000000')
        if not self.options['install']:
            self.args.extend(['--runcd', '--nogui'])

    def set_media_options_windows(self):
        cd_image = self.get_cd_image()
        reg_key = 'HKEY_CURRENT_USER\\Software\\Vision Thing\\' \
                'PSEmu Pro\\CDR\\MoobyCDR'
        RegistryHelper.delete_key(reg_key)
        RegistryHelper.create_key(reg_key)
        RegistryHelper.set_string(reg_key, 'autorun', cd_image)
        RegistryHelper.set_string(reg_key, 'lastrun', cd_image)

    def get_cd_image(self):
        cdroms = self.sort_cdroms(self.cdroms)
        return cdroms[0]
        #try:
        #    cd = self.configuration['CD-ROM DRIVE']
        #except KeyError:
        #    cd = 'CD'
        #for ext in ['.bin', '.img']:
        #    p = os.path.join(self.configuration.get_data_dir(), cd + ext)
        #    if os.path.exists(p):
        #        return p
        #raise Exception("No CD image found")

    def set_other_options(self):
        if fs.windows:
            return self.set_other_options_windows()
        bios, bios_dir = self.get_bios()
        mcd1, mcd2 = self.get_memory_cards()
        plugins_dir = pyapp.plug.get_plugin('net.fsolheim.emulator.pcsx'
                ).program_dir_path
        #plugins_dir = os.path.join(pyapp.plug.get_plugin(
        #        'net.fsolheim.emulator.pcsx').program_dir_path, 'plugins')
        self.pcsx_ini.append("[dir]")
        self.pcsx_ini.append("plugins=" + plugins_dir)
        self.pcsx_ini.append("bios=" + bios_dir)
        self.pcsx_ini.append("")
        self.pcsx_ini.append("[plugin]")
        self.pcsx_ini.append("pad1=libDFInput.so")
        self.pcsx_ini.append("pad2=libDFInput.so")
        self.pcsx_ini.append("spu=libDFSound.so")
        self.pcsx_ini.append("gpu=libDFXVideo.so")
        self.pcsx_ini.append("cdr=libDFBinImage2.so")
        self.pcsx_ini.append("")
        self.pcsx_ini.append("[bios]")
        self.pcsx_ini.append("bios=SCPH1001.BIN")
        self.pcsx_ini.append("")
        #self.pcsx_ini.append("loadcdbios=0")
        self.pcsx_ini.append("[cpu]")
        self.pcsx_ini.append("cdda=0")
        self.pcsx_ini.append("xa=0")
        self.pcsx_ini.append("")
        self.pcsx_ini.append("[memcard]")
        self.pcsx_ini.append("memcard1=" + mcd1)
        self.pcsx_ini.append("memcard2=" + mcd2)
        self.pcsx_ini.append("")

    def set_other_options_windows(self):
        bios, bios_dir = self.get_bios()
        mcd1, mcd2 = self.get_memory_cards()
        reg_key = 'HKEY_CURRENT_USER\\Software\\Pcsxr'
        RegistryHelper.delete_key(reg_key)
        RegistryHelper.create_key(reg_key)
        RegistryHelper.set_string(reg_key, 'Bios', bios)
        RegistryHelper.set_string(reg_key, 'BiosDir', bios_dir + '\\')
        #RegistryHelper.set_string(reg_key, 'Cdr', 'cdrmooby2.dll')
        RegistryHelper.set_integer(reg_key, 'Cdda', 0)
        RegistryHelper.set_string(reg_key, 'Cdr', 'cdrsapu.dll')
        RegistryHelper.set_integer(reg_key, 'Cpu', 0)
        RegistryHelper.set_integer(reg_key, 'Debug', 0)
        RegistryHelper.set_string(reg_key, 'Gpu', 'gpuPeopsSoft.dll')
        RegistryHelper.set_string(reg_key, 'Lang', 'English')
        RegistryHelper.set_string(reg_key, 'Mcd1', mcd1)
        RegistryHelper.set_string(reg_key, 'Mcd2', mcd2)
        RegistryHelper.set_integer(reg_key, 'MDec', 0)
        RegistryHelper.set_string(reg_key, 'Net', 'Disabled')
        #RegistryHelper.set_string(reg_key, 'Pad1', 'padSeguDIJoy.dll')
        #RegistryHelper.set_string(reg_key, 'Pad2', 'padSeguDIJoy.dll')
        RegistryHelper.set_string(reg_key, 'Pad1', 'padSSSPSX.dll')
        RegistryHelper.set_string(reg_key, 'Pad2', 'padSSSPSX.dll')
        #RegistryHelper.set_string(reg_key, 'PluginsDir', 'Plugins\\')
        RegistryHelper.set_string(reg_key, 'PluginsDir',
                pyapp.plugins.get_plugin('no.fengestad.emulator.pcsx').get_bin_dir()
                + '\\plugins\\')
        RegistryHelper.set_integer(reg_key, 'PsxAuto', 1)
        RegistryHelper.set_integer(reg_key, 'PsxOut', 0)
        RegistryHelper.set_integer(reg_key, 'PsxType', 0)
        RegistryHelper.set_integer(reg_key, 'RCntFix', 0)
        RegistryHelper.set_integer(reg_key, 'Sio', 0)
        RegistryHelper.set_integer(reg_key, 'SlowBoot', 1)
        RegistryHelper.set_string(reg_key, 'Spu', 'spuPeopsDSound.dll')
        RegistryHelper.set_integer(reg_key, 'SpuIrq', 0)
        RegistryHelper.set_integer(reg_key, 'VSyncWA', 0)
        RegistryHelper.set_integer(reg_key, 'Xa', 0)

    def get_memory_cards(self):
        #state_dir = self.get_state_dir(create=True)
        state_dir = self.context.game.state_dir
        return (os.path.join(state_dir, 'Memory Card 1.mcr'),
                os.path.join(state_dir, 'Memory Card 2.mcr'))

    def get_region(self):
        if self.get_game_refresh_rate() == 50.0:
            return 'EUROPE'
        return 'USA'

    def get_bios(self):
        #bios_dir = self.get_resource_dir('PlayStation NTSC BIOS')
        #if fs.windows:
        #    bios_dir += os.sep
        #return ('SCPH1001.BIN', bios_dir)

        # FIXME: CHECK 
        if self.get_region == 'EUROPE':
            bios_dir = self.prepare_dir('pse', [self.firmware_dir])
            return 'ps-20e.bin', bios_dir
        else:
            bios_dir = self.prepare_dir('psu', [self.firmware_dir])
            return 'ps-20a.bin', bios_dir


if fs.windows:
    import win32api
    import win32con
    import win32gui


SSSPSX_SELECT = 0
SSSPSX_START = 6
SSSPSX_UP = 8
SSSPSX_RIGHT = 10
SSSPSX_DOWN = 12
SSSPSX_LEFT = 14
SSSPSX_L2 = 16
SSSPSX_R2 = 18
SSSPSX_L1 = 20
SSSPSX_R1 = 22
SSSPSX_TRIANGLE = 24
SSSPSX_CIRCLE = 26
SSSPSX_CROSS = 28
SSSPSX_SQUARE = 30
SSSPSX_ANALOG = 32
SSSPSX_DUALSHOCK = 0xA8 / 2 # 01 00 = On, 00 00 = Off


class SSSPSXInputMapper(GameController.InputMapper):

    def axis(self, axis, positive):
        value = axis * 2
        if positive:
            value += 1
        return value, 0x20 + self.device.index

    def hat(self, hat, direction):
        value = {
            'left': 3,
            'right': 1,
            'up': 0,
            'down': 2,
        }[direction]
        return value, 0x30 + self.device.index

    def button(self, button):
        return button, 0x10 + self.device.index

    def key(self, key):
        return key.dinput_code, 0x00


class RegistryHelper:

    @classmethod
    def split_key(cls, key):
        rootkey, subkey = key.split('\\', 1)
        if rootkey == 'HKEY_CURRENT_USER':
            return win32con.HKEY_CURRENT_USER, subkey
        raise Exception("Unrecognized registry key: " + repr(key))

    @classmethod
    def delete_key(cls, key):
        keyhandle, subkey = cls.split_key(key)
        try:
            win32api.RegDeleteKey(keyhandle, subkey)
        except win32api.error, e:
            if e[0] == 2:
                return  # Cannot find key, OK
            raise e

    @classmethod
    def create_key(cls, key):
        keyhandle, subkey = cls.split_key(key)
        win32api.RegCreateKey(keyhandle, subkey)

    @classmethod
    def set_string(cls, key, item, value):
        cls.set_value(key, item, win32con.REG_SZ, value)

    @classmethod
    def set_binary(cls, key, item, value):
        v = ""
        for val in value:
            v += chr(val)
        cls.set_value(key, item, win32con.REG_BINARY, v)

    @classmethod
    def set_integer(cls, key, item, value):
        cls.set_value(key, item, win32con.REG_DWORD, value)

    @classmethod
    def set_value(cls, key, item, value_type, value):
        keyhandle, subkey = cls.split_key(key)
        keyhandle = win32api.RegOpenKeyEx(keyhandle, subkey, 0,
                win32con.KEY_ALL_ACCESS)
        win32api.RegSetValueEx(keyhandle, item, 0, value_type, value)
        win32api.RegCloseKey(keyhandle)

