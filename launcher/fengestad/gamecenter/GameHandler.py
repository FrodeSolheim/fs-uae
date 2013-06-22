from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import sys
import pkg_resources
from fengestad import fs
from fengestad.input.inputdevice import InputDevice
from fengestad.input.keyboard import Keyboard
#from .settings import Settings
from .gamecenter import GameCenter
from .nameutil import NameUtil
from .platform import Platform
from .config import Config
from .gamecenterutil import GameCenterUtil
from .refreshratetool import RefreshRateTool
from .emulatorcontrol import EmulatorControl
from fengestad.gamecenter.resources import resources, logger, ngettext, _


EXCEPTION = "EXCEPTION"


class ProcessEmulatorControl(EmulatorControl):

    def __init__(self, process):
        EmulatorControl.__init__(self)
        self.process = process

    def wait(self):
        return self.process.wait()

    def abort(self, terminate=False):
        print("ProcessEmulatorControll.abort, terminate =", terminate)
        try:
            if terminate:
                self.process.terminate()
            else:
                self.process.kill()
        except Exception:
            import traceback
            traceback.print_exc()


class GameOptions():
    pass


class GameConfiguration():

    def __init__(self, context, values):
        self.context = context
        self.values = values

    def get_data_dir(self):
        try:
            data_dir_name = self.values['data_directory']
            return os.path.join(self.context.gamedata_dir,
                    data_dir_name)
        except KeyError:
            pass
        try:
            version = self.values['version']
            version_dir = os.path.join(self.context.gamedata_dir,
                    version)
            if os.path.exists(version_dir):
                return version_dir
        except KeyError:
            pass
        return self.context.gamedata_dir

    def get_state_dir_name(self):
        # FIXME: if DATA DIRECTORY
        if self.context.get_major_revision() < 2:
            return u"%s" % (self.context.get_full_id())
        else:
            return u"%s R%d" % (self.context.get_full_id(),
                    self.context.get_major_revision())

    def get(self, key):
        return self.values[key]

    def __getitem__(self, key):
        return self.values[key]


class LaunchError(RuntimeError):
    pass


#class AttributeHolder(object):
#    pass


class Option(object):

    def __init__(self):
        self.priority = 0.0
        self.group = False
        self.title = "Option Title"
        self.subtitle = ""

    def activate(self):
        pass

    @classmethod
    def create_play_option(cls):
        return PlayOption()
        
    @classmethod
    def create_config_option(cls):
        return ConfigOption()

    @classmethod
    def create_screen_option(cls):
        return ScreenOption()

    @classmethod
    def create_aspect_option(cls):
        return AspectOption()

    @classmethod
    def create_group(cls, title, priority=0.0):
        option = Option()
        option.priority = priority
        option.title = title
        option.group = True
        return option


class PlayOption(Option):

    def __init__(self):
        Option.__init__(self)
        self.priority = -2.0
        self.title = "Play Game"
        self.subtitle = "Play Game"

    def activate(self):
        print("PlayItem.activate")
        return 'PLAY'


class ConfigOption(Option):

    def __init__(self):
        Option.__init__(self)
        self.priority = -1.0
        self.title = "CONFIGURATION NAME"
        self.subtitle = "Choose Configuration"

    def activate(self):
        print("ChooseConfigItem.activate")
        return [
        
        ]


class AspectOption(Option):

    def __init__(self):
        Option.__init__(self)
        self.priority = 98.0
        self.title = "FILL ENTIRE DISPLAY"
        self.subtitle = "Display Configuration"

    def activate(self):
        print("AspectItem.activate")
        return [
        
        ]

class ScreenOption(Option):

    def __init__(self):
        Option.__init__(self)
        self.priority = 99.9
        self.title = "FULL-SCREEN 50HZ V"
        self.subtitle = "Screen Configuration"

    def activate(self):
        print("DisplayItem.activate")
        return [
        
        ]


class Input(object):
    
    def __init__(self, name, type, description):
        self.name = name
        self.type = type
        self.description = description
        self.device = None
        self.device_id = None
        self.device_config = None


class InputMapper(object):

    def __init__(self, input, mapping):
        self.input = input
        self.device = input.device
        self.mapping = mapping

    def iteritems(self):
        input = self.input
        if not input.device_config:
            return
        for native_button, game_button in input.device_config.iteritems():
            try:
                input_key = self.mapping[game_button]
            except KeyError:
                pass
            else:
                input_value = self.calc_input(native_button)
                #print("---->", input_key, input_value)
                #f.write('{input_key} = "{input_value}"\n'.format(
                #        input_key=input_key,
                #        input_value=input_value))
                if input_value is not None:
                    yield input_key, input_value
    
    def calc_input(self, value):
        parts = value.lower().split('_')
        if parts[0] == 'axis':
            axis = int(parts[1])
            positive = parts[2] == 'pos'
            return self.axis(axis, positive)
        elif parts[0] == 'hat':
            hat = int(parts[1])
            direction = parts[2]
            return self.hat(hat, direction)
        elif parts[0] == 'button':
            button = int(parts[1])
            return self.button(button)
        elif parts[0] == 'key':
            key_name = value.split('_', 1)[1].lower()
            if key_name == "rctrl" and fs.macosx:
                print("using ralt instead of rctrl on Mac")
                key_name = "ralt"
            key = Keyboard.key(key_name)
            return self.key(key)
        elif parts[0] == 'mouse':
            button = 0
            axis = 0
            positive = False
            if parts[1] == 'left':
                button = 1
            elif parts[1] == 'right':
                button = 2
            elif parts[1] == 'middle':
                button = 3
            else:
                axis = int(parts[1])
                positive = parts[2] == 'pos'
            return self.mouse(button, axis, positive)
        return None
        #raise Exception("Invalid input value: {0}".format(value))


class GameHandler(object):
    Option = Option
    InputMapper = InputMapper

    @classmethod
    def create_for_game_context(cls, context):
        """
        for ext in pyapp.ext.ExtensionHook(
                'exthook:no.fengestad.game.system/game-controller'):
            if ext.platform == context.game.platform:
                #return ext.object(context)
                return ext.object.create(context)
        else:
            raise Exception("Could not find game controller for "
                    "platform '{0}'".format(context.game.platform))
        """
        """
        cmp_platform = NameUtil.create_cmpname(context.game.platform)
        for entry_point in pkg_resources.iter_entry_points(
                'fengestad.gamecenter.handler', cmp_platform):
            # return first available, load should return a GameHandler
            # class
            return entry_point.load().create(context)
        else:
            raise Exception("Could not find game handler for "
                    "platform '{0}'".format(context.game.platform))
        """
        assert context.game.platform == "Amiga" or context.game.platform == "CD32" or context.game.platform == "CDTV"
        from fengestad.game.amiga.AmigaGameHandler import AmigaGameHandler
        return AmigaGameHandler(context)

    @classmethod
    def create(cls, context):
        return cls(context)

    """
    @classmethod
    def create(cls, platform, **kwargs):
        for ext in pyapp.ext.ExtensionHook(
                'exthook:no.fengestad.game.system/game-controller'):
            if ext.platform == platform:
                #return ext.object(context)
                return ext.object(platform=platform, **kwargs)
        else:
            raise Exception("Could not find game controller for "
                    "platform '{0}'".format(context.game.platform))
        #return cls(platform=platform, **kwargs)
    """

    #@property
    #def platform(self):
    #    return self.context.game.platform

    def __init__(self, context=None): #, platform="", file=""):
        
        self.abort_count = 0
        self.__vsync = False
        self.context = context

        self._emulator_exited = False
        self._emulator_process = None
        #self.options = GameOptions()

    # methods meant to be overridable...

        self.options = []
        self.options.append(Option.create_group("Game Control", -2.5))
        self.context.play_option = Option.create_play_option()
        self.options.append(self.context.play_option)

        # self.options.append(Option.create_group("Configurations (X)", -1.5))
        # self.context.config_option = Option.create_config_option()
        # self.options.append(self.context.config_option)
        #
        # self.options.append(Option.create_group("Options", -0.5))
        #
        # self.options.append(Option.create_group("Display Options", 90.0))
        # self.context.aspect_option = Option.create_aspect_option()
        # self.options.append(self.context.aspect_option)
        # self.context.screen_option = Option.create_screen_option()
        # self.options.append(self.context.screen_option)

        self.inputs = []
        self.init_game()

    def get_supported_filters(self):
        return {}

    @fs.memoize
    def read_option(self, option, type, default):
        option = '--' + option + '='
        for arg in sys.argv:
            if arg.startswith(option):
                value = arg[len(option):]
                if type == 'bool':
                    value = value.lower().strip()
                    if value in ['true', 'yes', '1']:
                        return True
                    else:
                        return False
                elif type == 'string':
                    return value
                else:
                    raise Exception("unknown option type")
        return default

    def get_option(self, option):
        if option == 'install':
            return self.read_option(option, 'bool', False)
        if option == 'filters':
            return self.read_option(option, 'string', '')
        if option == 'stretch':
            return self.read_option(option, 'bool', True)
        if option == 'fullscreen':
            return self.read_option(option, 'bool', True)
        if option == 'smooth':
            return self.read_option(option, 'bool', True)
        if option == 'vsync':
            return self.read_option(option, 'bool', True)
        if option == 'fsaa':
            return Config.get_int("video/fsaa", 0)
        print("\n\nWARNING: unknown option", repr(option), "\n\n")
        return None

    def get_game_refresh_rate(self):
        """Default algorithm for auto-selecting refresh rate for game.

        Override in subclasses to tune algorithm for specific platforms.
        """
        # FIXME: Approx.
        NTSC_FRAMERATE = 60.0
        PAL_FRAMERATE = 50.0
        config_name = self.context.game.config['name'].upper()
        print("get_game_refresh_rate, config_name = ", config_name)
        if 'NTSC' in config_name:
            return NTSC_FRAMERATE
        if 'PAL' in config_name:
            return PAL_FRAMERATE
        if 'USA' in config_name:
            return NTSC_FRAMERATE
        if 'WORLD' in config_name:
            return NTSC_FRAMERATE
        if 'EUROPE' in config_name:
            return PAL_FRAMERATE
        if 'BRAZIL' in config_name:
            return NTSC_FRAMERATE
        #if 'EU' in config_name:
        #    return PAL_FRAMERATE
        #if 'US' in config_name:
        #    return NTSC_FRAMERATE
        return None

    def cleanup(self):
        """Override in subclasses to clean up after emulator."""
        pass

    def post_start_emulator(self, process):
        pass

    # methods not meant to be overriden...

    def configure_filter(self):
        supported = self.get_supported_filters()
        for name in self.get_option('filters').split(','):
            name = name.strip()
            for item in supported:
                if item['name'] == name:
                    return item
        if len(supported) > 0:
            return supported[0]
        return None

    #def set_options(self, **kwargs):
    #    print("WARNING: GameHandler.set_options is now an no-op")

    def get_vsync(self):
        return self.__vsync

    def set_vsync(self, vsync):
        print("GameHandler - set vsync to", vsync)
        self.__vsync = vsync

    def configure_vsync(self):
        print("\n" + "-" * 79 + "\n" + "CONFIGURE VSYNC")
        #print("^" * 80)
        if self.get_option('vsync'):
            try:
                game_refresh = float(self.context.game.config['refreshrate'])
            except Exception:
                game_refresh = self.get_game_refresh_rate()
                #if self.context.game.config.get('system', '') == 'NTSC':
                #    game_refresh = 60.0
                #else:
                #    game_refresh = 50.0
            rrtool = RefreshRateTool(game_platform=self.context.game.platform,
                    game_refresh=game_refresh)
            rrtool.set_best_mode()
            if rrtool.allow_vsync():
                print("setting vsync to true")
                self.set_vsync(True)
                return rrtool.get_display_refresh()
        else:
            print("vsync is not enabled")
        print("setting vsync to false")
        self.set_vsync(False)
        return False

    def get_emulator_process(self):
        return self._emulator_process

    # FIXME: cache
    def resources_dirs(self):
        dirs = []
        p = os.path.dirname(self.context.game.file)
        while True:
            path = os.path.join(p, 'Resources')
            #print(path)
            if os.path.exists(path):
                dirs.append(path)
            path = os.path.join(p, 'resources')
            if os.path.exists(path):
                dirs.append(path)
            old_p = p
            p = os.path.dirname(p)
            if p == old_p:
                break
        return dirs

    def resource_file(self, name):
        # find platform-specific resource
        for dir in self.resources_dirs():
            path = os.path.join(dir, self.platform, name)
            if os.path.exists(path):
                return path
        # find resource outside platform-specific dirs
        for dir in self.resources_dirs():
            path = os.path.join(dir, name)
            if os.path.exists(path):
                return path
        #return None
        raise Exception("did not find resource file " + repr(name))

    def run_game(self, configuration={}, **options):
        print("\n" + "-" * 79 + "\n" + "GAMECONTROLLER RUN GAME")
        self._do_run(configuration, **options)
        #try:
        #    self._do_run(configuration, **options)
        #except Exception, e:
        #    print
        #    print("ERROR RUNNING GAME")
        #    print(e)
        #    import traceback
        #    traceback.print_exc()
        #    logger.exception("Error running game")
        #    self.on_error(message=pyapp.unicode_safe(e))

    def get_config(self, key, default=EXCEPTION):
        value = self.context.game.config.get(key, default)
        if value is EXCEPTION:
            raise Exception("missing config " + key)
        return value

    def _do_run(self, configuration, **options):
        #self._base_temp_dir = tempfile.mkdtemp(prefix='fsms-')
        self.configuration = GameConfiguration(self.context, configuration)
        print("Configuration:", self.configuration)
        env = os.environ.copy()
        rect = None #self.get_window_rect()
        if self.context.config.fullscreen:
            #if fs.linux:
            rect = [self.context.system.screen_x,
                self.context.system.screen_y, 
                self.context.system.screen_width,
                self.context.system.screen_height]
        if self.get_option('stretch'):
            env['VIDEO_STRETCH'] = '1'
        else:
            env.pop('VIDEO_STRETCH', None)
        if rect is not None:
            env['WINDOW_POS'] = '{0},{1}'.format(rect[0], rect[1])
            env['WINDOW_SIZE'] = '{0},{1}'.format(rect[2], rect[3])
            if fs.linux:
                env['SDL_VIDEO_WINDOW_POS'] = '{0},{1}'.format(rect[0], rect[1])
        else:
            env.pop('WINDOW_POS', None)
            env.pop('WINDOW_SIZE', None)
            env.pop('SDL_VIDEO_WINDOW_POS', None)

        if self.get_vsync():
            # needed for vsync on Linux / nVIDIA
            env['__GL_SYNC_TO_VBLANK'] = '1'
        else:
            env['__GL_SYNC_TO_VBLANK'] = '0'

        env.update(self.env)
        self.env = env

        #self.on_status(status=_("Preparing"), sub_status=_("Initializing"))
        #self.prepare_game()
        #self.on_status(status=_("Starting"),
        #        sub_status=_("Configuring Emulator"))
        #self.on_status(status=_("Starting"),
        #        sub_status=_("Starting Emulator"))
        self.emulator_control = self.start_emulator(env)
        retval = self.emulator_control.wait()
        logger.debug("emulator return value was {0}".format(retval))
        # sleep to prevent cleanup
        #import time
        #time.sleep(1000)
        self._cleanup()

    def start_emulator(self, env):
        #import time
        #while True:
        #    time.sleep(100)
        
        process = self.create_emulator_process(env)
        os.environ['KGS_EMULATOR_PID'] = str(process.pid)
        self._emulator_process = process
        print("PROCESS =", process)
        print("- signalling on_start")
        #self.on_start(context=self.context, process=process,
        #        command_input=self._get_temp_file('KGS_INTEGRATION_INPUT'),
        #        command_output=self._get_temp_file(
        #        'KGS_INTEGRATION_OUTPUT'))
        self.post_start_emulator(process)
        return ProcessEmulatorControl(process)

    def abort(self):
        print("GameHandler.abort")
        self.abort_count += 1
        try:
            emulator_control = self.emulator_control
        except AttributeError, e:
            print(e)
        else:
            print("calling abort on emulator_control")
            emulator_control.abort(terminate=(self.abort_count >= 3))

    def run_emulator(self, name, cwd=None):
        """Run an emulator with a given name for this game controller."""
        #for entry_point in pkg_resources.iter_entry_points(
        #        'fengestad.gamecenter.emulator', name):
        #    # return first available, load should return a callable
        #    run_func = entry_point.load()
        #    break
        #else:
        #    raise Exception("Could not find emulator {0}".format(name))
        #return run_func(args=self.args, env=self.env)
        return GameCenterUtil.run_program(name, args=self.args, env=self.env,
                close_fds=True, cwd=cwd)

    # FIXME: REMOVE THIS INTERMEDIATE
    def create_emulator_process(self, env):
        return self.run() 

    def init_game(self):
        self.input_device_order = 'SDL'
        self.input_mapping_multiple = True
        self.init()
        self.options = [x[1] for x in sorted([(y.priority, y) \
                for y in self.options])]

    def init(self):
        pass

    def init_input(self):
        self.inputs = []

    def create_input(self, name="", type="", description=""):
        input = Input(name, type, description)
        return input

    def prepare_game(self):
        self.args = []
        self.env = {}
        #if self.context.game.file.endswith('.game'):
        #    self.prepare_game_file()
        self.prepare()

    def prepare_game_file(self):
        print("Prepare game file")
        game_dir = self.context.temp.dir('gamedata')
        GameCenterUtil.unpack_zip(self.context.game.file, game_dir)
        #new_file = None
        for name in os.listdir(game_dir):
            path = os.path.join(game_dir, name)
            _n, ext = os.path.splitext(name)
            ext = ext.lower()
            if not ext in Platform.extensions:
                continue

            number = NameUtil.find_number(path)
            if number and number != 1:
                # only consider 1st disk/disc in multi-part game
                continue
            print("<<< old game file:", self.context.game.file)
            self.context.game.file = path
            print(">>> new game file:", self.context.game.file)
            print("\n\n\n\nFIXME: IMPORTANT: RUN JAVASCRIPT ON PATHS TO CORRECTLY CHECK DISK NUMBER\n\n\n\n")

            return
        raise Exception("could not find game file after unpacking game")

    def prepare_firmware(self, name):
        dirs = GameCenter.game_dirs[:]
        # Also check Firmware sub-dirs
        for dir in GameCenter.game_dirs:
            dirs.append(os.path.join(dir, "Firmware"))
        return self.prepare_dir(name, dirs)

    def prepare_dir(self, name, dirs = []):
        if not dirs:
            dirs = GameCenter.game_dirs
        for dir in dirs:
            path = os.path.join(dir, name)
            if os.path.exists(path):
                return path
            path = os.path.join(dir, name + u'.zip')
            if os.path.exists(path):
                temp_dir = self.context.temp.dir(name)
                GameCenterUtil.unpack(path, temp_dir)
                return temp_dir
        raise Exception("Could not find {0} or {1}".format(repr(name),
                repr(name + u'.zip')))

    def prepare(self):
        pass
    
    def configure_game(self):
        self.configure_emulator()

    # FIXME: REMOVE INDERMEDIATE configure_emulator
    def configure_emulator(self):
        self.__configure_input()
        self.configure()

    def get_gamedata_temp_dir(self):
        if not self._gamedata_temp_dir:
            #self._gamedata_temp_dir = tempfile.mkdtemp(prefix='fsgs-')
            self._gamedata_temp_dir = self._get_temp_dir('gamedata')
            self._gamedata_temp_dir = fs.unicode_path(
                    self._gamedata_temp_dir)
        return self._gamedata_temp_dir

    def _find_game_rom(self, extensions):
        data_dir = self.configuration.get_data_dir()
        temp_dir = self.get_gamedata_temp_dir()
        for item in os.listdir(data_dir):
            itempath = os.path.join(data_dir, item)
            if item.endswith(".zip"):
                # FIXME
                self._unzip_flat(itempath, temp_dir)
                data_dir = temp_dir
            elif item.endswith(".7z"):
                self.unpack(itempath, temp_dir)
        rom_path = None
        for item in os.listdir(temp_dir):
            itemlower = item.lower()
            name, ext = os.path.splitext(itemlower)
            if ext in extensions:
                rom_path = os.path.join(temp_dir, item)
                break
        else:
            raise Exception("Could not find ROM for game %s" % (
                    self.game_info.title,))
        return rom_path

    def _cleanup(self):
        logger.debug("GameHandler._cleanup")

        # Emulator-specific cleanup
        self.cleanup()
        
        state_dir = self.context.game.state_dir
        if os.path.isdir(state_dir):
            if len(os.listdir(state_dir)) == 0:
                logger.info("state dir has no items; removing {0}".format(
                        state_dir))
                try:
                    os.rmdir(state_dir)
                except Exception, e:
                    # this is not important, just ignore the error
                    print(repr(e))

    def __configure_input(self):
        print("\n" + "-" * 79 + "\n" + "CONFIGURE INPUT")
        #device_order = self.get
        #print("device order is", self.input_device_order)
        devices = InputDevice.get_devices(
                device_order=self.input_device_order)
        for input in self.inputs:
            print("input port:", input.device_id)
            input.device = None
            input.device_config = None
            for device in devices:
                if input.device_id == device.id:
                    try:
                        input.device_config = device.configure(input.type,
                                multiple=self.input_mapping_multiple)
                    except Exception, e:
                        print("error configuring device:", repr(e))
                    else:
                        print("configured device", device)
                        input.device = device
                    break
