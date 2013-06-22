from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import re
import sys
import time
import json
import tempfile
import shutil
import subprocess
import ConfigParser
from fengestad.input.inputdevice import InputDevice
from .gamecenter import GameCenter
from .database import Database
from .platform import Platform
from .nameutil import NameUtil
from fengestad import fs

from fsgs import fsgs

class SystemContext(object):
    
    def __init__(self):
        overscan = fs.config_string('Display/Overscan', '0,0,0,0')
        try:
            overscan = overscan.split(',')
            self.overscan = [int(x.strip()) for x in overscan]
            if self.overscan != [0, 0, 0, 0]:
                print("using overscan", self.overscan)
        except Exception, e:
            print("error parsing overscan from config:", repr(e))
            self.overscan = [0, 0, 0, 0]
        #rect[0] = rect[0] + overscan[0]
        #rect[1] = rect[1] + overscan[1]
        #rect[2] = rect[2] - overscan[0] - overscan[2]
        #rect[3] = rect[3] - overscan[1] - overscan[3]
        self.screen_x = self.overscan[0]
        self.screen_y = self.overscan[1]

    @property
    def screen_width(self):
        return self.screen_size[0]

    @property
    def screen_height(self):
        return self.screen_size[1]

    @property
    @fs.memoize
    def screen_size(self):
        #import wx
        #pyapp.app.require_wx_app()
        #size = wx.DisplaySize()
        #if size[0] > size[1] * 2:
        #    # Assume dual monitor setup..
        #    size = size[0] // 2, size[1]
        #return (size[0] - self.overscan[0] - self.overscan[2],
        #        size[1] - self.overscan[1] - self.overscan[3])
        # FIXME
        print("FIXME: MUST FIX screen_size")
        return (1024, 576)


class TempContext(object):

    def __init__(self):
        self.temp_dir_names = {}
        self.temp_dirs = []

    def __del__(self):
        if '--debug' in sys.argv:
            return
        delete_list = self.get_delete_list()
        new_delete_list = delete_list[:]
        for path in delete_list:
            print("DELETE OLD TEMP FILE/DIR", repr(path))
            if not '-fsgc' in path:
                print("not a FSGC path", skipping)
                new_delete_list.remove(path)
                continue
            if os.path.exists(path):
                try:
                    shutil.rmtree(path)
                    print("deleted successfully")
                except Exception, e:
                    print(repr(e))
                    print("still cannot delete...")
                    continue
            new_delete_list.remove(path)
        self.set_delete_list(new_delete_list)

        for path in self.temp_dirs:
            print("deleting", repr(path))
            try:
                shutil.rmtree(path)
            except Exception, e:
                print("could not delete...")
                print(e)
                #delete_list = self.get_delete_list()
                #delete_list.append(path)
                #self.set_delete_list(delete_list)
                self.add_to_delete_list(path)

    def add_to_delete_list(self, path):
        k = 0
        while True:
            if not fs.config_string('DeletePath/{0}'.format(k)):
                break
            k += 1
        pyapp.user.ini.set('DeletePath/{0}'.format(k), path)
        print("added", repr(path), "to delete list")

    def get_delete_list(self):
        delete_list = []
        k = 0
        while True:
            path = fs.config_string('DeletePath/{0}'.format(k))
            if not path:
                break
            delete_list.append(path.strip())
            k += 1
        return delete_list

    def set_delete_list(self, delete_list):
        try:
            pyapp.user.ini.remove_section('DeletePath')
        except Exception:
            pass
        for i, path in enumerate(delete_list):
            pyapp.user.ini.set('DeletePath/{0}'.format(i), path)

    def dir(self, name=None):
        if name:
            try:
                return self.temp_dir_names[name]
            except KeyError:
                pass
            path = tempfile.mkdtemp(suffix='-' + name + '-fsgc')
        else:
            path = tempfile.mkdtemp(suffix='-fsgc')
        if name:
            self.temp_dir_names[name] = path
        self.temp_dirs.append(path)
        return path

    def file(self, name=None):
        dir = self.dir(name)
        name = name or 'temp'
        return os.path.join(dir, name)
        


class ConfigContext(object):
    def __init__(self):
        self.fullscreen = True
        self.install = False
        self.setup = False
        self.stretch = False
        self.vsync = True
        self.filters = ['auto']


class InputContext(object):
    def __init__(self, platform):
        self.platform = platform

    # FIXME: DEPRECATED
    def get_devices(self, min_count, max_count=None,
            device_order='SDL', device_list=None):
        return InputDevice.get_devices(self.platform, min_count, max_count,
                device_order, device_list, version=1)

"""
# FIXME: PUT IN COMMON MODULE
# LATEST AS OF 2011-02-28
def extract_names(path):
    game_name = ""
    config_name = ""
    name = os.path.basename(path)
    # find real case of file name
    for item in os.listdir(os.path.dirname(path)):
        if os.path.normcase(item) == os.path.normcase(name):
            name = item
            break
    name, ext = os.path.splitext(name)
    name = re.sub('\\([Dd][Ii][Ss][Kk][^)]*\\)', '', name)
    # Reel 1, 2, 3 used by Cinemaware games
    name = re.sub('\\([Rr][Ee][Ee][Ll][^)]*\\)', '', name)
    # disregard CD numbers (CD 1, etc)
    name = re.sub('\\([Cc][Dd][^-)]*\\)', '', name)
    name = re.sub('[ ]+', ' ', name)
    # find end of name (and start of config name)
    pos = name.find('(')
    pos2 = name.find('[')
    if pos > 0:
        if pos2 > 0 and pos2 < pos:
            pos = pos2
    if pos > 0:
        game_name = name[:pos].strip()
        config_name = name[pos:]
        config_name = config_name.replace(',', ', ')
        config_name = config_name.replace(')', ', ')
        config_name = config_name.replace(']', ', ')
        config_name = config_name.replace('(', '')
        config_name = config_name.replace('[', '')
        config_name = config_name.strip(' ,')
        config_name = re.sub('[ ]+', ' ', config_name)
    else:
        game_name = name.strip()
    if game_name.lower().endswith(', the'):
        game_name = 'The ' + game_name[:-5]
        game_name = game_name.strip()
    elif game_name.lower().endswith(', a'):
        game_name = 'A ' + game_name[:-3]
        game_name = game_name.strip()
    return game_name, config_name
"""


# FIXME: PUT IN COMMON MODULE
def file_system_name(name):
    name = name.replace(':', ' - ')
    name = name.replace('  ', ' ')
    name = name.replace('/', '_')
    name = name.replace('|', '_')
    name = name.replace('\\', '_')
    name = name.replace('<', '_')
    name = name.replace('>', '_')
    name = name.replace('*', '_')
    name = name.replace('"', '\'')
    while name.endswith('.'):
        name = name[:-1]
    name = name.strip()
    return name

"""
class GameConfig(object):

    #def __init__(self):
    #    self._data = {}

    def get(self, key, default=None):
        key = key.lower()
        #if hasattr(self, key):
        if key in self.__dict__:
            return getattr(self, key)
        #if key in self._data:
        #    return self._data[key]
        return default

    def keys(self):
        #return self._data.keys()
        names = []
        for name in dir(self):
            if name in ['get', 'keys']:
                continue
            if not name.startswith('_'):
                names.append(name)
        return names
        
    def __getattr__(self, key):
        try:
            return self.__dict__[key.lower()]
        except KeyError:
            return None

    def copy(self):
        return self.

    #def __setattr__(self, key, value):
    #    #if key == '_data':
    #    #    self.__dict__['_data'] = value
    #    #else:
    #    #    self._data[key.lower()] = value
    #    #    #self.__dict__[key.lower()] = value
    #    self.

    def __getitem__(self, key):
        #print("__getitem__", key)
        #return getattr(self, key.lower())
        #try:
        #    return self._data[key.lower()]
        #except KeyError:
        #    raise AttributeError(key)
        try:
            return getattr(self, key)
        except AttributeError:
            return None

    def __setitem__(self, key, value):
        #self._data[key.lower()] = value
        #self._data[key.lower()] = value
        setattr(self, key, value)
"""


class Game(object):
    def __init__(self, game_name, platform_name="", variant_name="",
            variant_uuid="", **kwargs):
        self.variant_name = variant_name
        self.variant_uuid = variant_uuid
        #self.id = ''
        #self.file = game_file
        #self.name, config_name = NameUtil.extract_names(game_file)
        self.config = {}
        #self.config["name"] = config_name
        self.platform = platform_name
        self.state_dir_name = None

    @property
    def state_dir(self):

        # FIXME: Use known folder SAVED_GAMES (or something) on Vista+
        #path = os.path.join(pyapp.user.home_dir(), 'Saved Games')
        path = GameCenter.save_dir
        #path = pyapp.user.documents_dir()
        """
        if self.state_dir_name:
            # add a '.' because extract_names expects a file ending
            name, config = NameUtil.extract_names(self.state_dir_name + u'.')
            if config:
                pass
            else:
                config = self.platform
            #if not config.startswith(self.platform):
            #    config = self.platform + u', ' + config
            #name = name + u' (' + config + ')'
        else:
            name = self.name
            #if self.config['name']:
            #    config = self.config['name']
            #else:
            #    config = self.platform
            config = self.variant_name
        if not config.startswith(self.platform):
            config = self.platform + u', ' + config
        name = name + u' (' + config + ')'
        """
        name = self.variant_name
        name = file_system_name(name)
        #path = os.path.join(path, self.platform, name)
        path = os.path.join(path, name)
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    """
    @property
    def platform(self):
        if self._platform:
            return self._platform
        name = self.file.upper()
        #print("self.file", self.file)
        while len(name) > 3:
            name = os.path.dirname(name)
            n = os.path.basename(name)
            #print(n)
            if n in Platform.aliases:
                self._platform = Platform.aliases[n]
                return self._platform
        raise Exception("Did not find platform")
        #return 'Super Nintendo'
    """

#    def set_platform(self, platform):
#        self._platform = platform


class GameContext(object):

    def __init__(self, game_name, platform_name, variant_name="",
            variant_uuid="", **kwargs):
        
        print("\n\nNew GameContext")
        print("game_name", game_name)
        print("platform_name", platform_name)
        print("variant_name", variant_name)
        print("variant_uuid", variant_uuid)
        #self._game_file = game_file
        self.config = ConfigContext()
        self.game = Game(game_name, platform_name, variant_name, variant_uuid,
                **kwargs)
        self.temp = TempContext()
        self.system = SystemContext()
        self.input = InputContext(self.game.platform)
        self.database = None
    #def __init__(self, metadata, gamedata_dir, resources={}):
    #    self.metadata = metadata
    #    self.gamedata_dir = gamedata_dir
    #    self.resources = resources

    def get_game_state_dir(self):
        return self.game.state_dir

    def get_temp_dir(self, name):
        return self.temp.dir(name)

    @fs.memoize
    def get_platform(self):
        #for tag in self.metadata['tags']:
        #    if tag[0] == 'Platform':
        #        return tag[1]
        #raise RuntimeError("No platform specified for game")
        return "Super Nintendo"

    @fs.memoize
    def get_year(self):
        #for tag in self.metadata['tags']:
        #    if tag[0] == 'Year':
        #        return tag[1]
        #raise RuntimeError("No year specified for game")
        return "1900"

    def get_full_id(self):
        return "{0} ({1}, {2})".format(self.metadata['id'],
                self.get_platform(), self.get_year())

    def get_major_revision(self):
        return int(self.metadata['revision'].split('.', 1)[0])

    #def get_resources_dir(self):
    #    return os.path.join(os.path.expanduser('~'), 'Resources')

    def get_resource_dir(self, resource):
        return self.resources[resource]

    def get_profile_dir(self):
        return os.path.join(os.path.expanduser('~'), 'Profile')

    @classmethod
    def create_for_game(cls, platform_name, game_name, config):
        print("\n" + "-" * 79 + "\n" + "CREATE GAME CONTEXT")
        print("for", platform_name, game_name, config)
        #platform_name = platform
        #game_name = name
        variant_uuid, variant_name = config[:2]
        #variant_id, variant_name, variant_uuid = config[:3]
        #self.done = False
        #self.status = ""
        #self.backtrace = None

        #from fsgs.GameDatabase import GameDatabase
        from fsgs.GameDatabaseClient import GameDatabaseClient
        game_database = fsgs.get_game_database()
        game_database_client = GameDatabaseClient(game_database)

        game_id = game_database_client.get_game_id(variant_uuid)
        config = game_database_client.get_final_game_values(game_id)

        """
        Database.cursor.execute("SELECT id, script FROM Game WHERE platform = ? "
                "AND name = ? AND config = ?", (platform_name,
                game_name, config_name))
        row = Database.cursor.fetchone()
        if not row:
            #self.status = "Did not find game in database (unexpectedly)"
            #self.done = True
            #return
            raise Exception("Did not find game in database (unexpectedly)")
        game_id = row[0]
        script = row[1]
        #self.name = row[0]
        #self.config_name = row[1]
        #self.platform = row[2]
        Database.cursor.execute("SELECT path FROM File, GameFile "
                "WHERE File.id = GameFile.file AND "
                "GameFile.game = ?", (game_id,))
        row = Database.cursor.fetchone()
        if not row:
            raise Exception("Did not find game in database (unexpectedly)")
            #self.status = "Did not find game in database (unexpectedly)"
            #self.done = True
            #return
        relative_path = row[0]
        for dir in GameCenter.game_dirs:
            absolute_path = os.path.join(dir, relative_path)
            if os.path.exists(absolute_path):
                game_file = absolute_path
                break
        else:
            raise Exception("Could not find game file " + repr(relative_path))
        config = {}
        print("SELECT key, value FROM Config "
                "WHERE game = {0}".format(game_id))
        Database.cursor.execute("SELECT key, value FROM Config "
                "WHERE game = ?", (game_id,))
        for row in Database.cursor:
            print(row[0], '=', row[1])
            config[row[0]] = row[1]
        #self.done = False
        """

        # FIXME: game_file is not enough to uniquely identify configuration
        context = GameContext(game_name, platform_name=platform_name,
                variant_name=variant_name, variant_uuid=variant_uuid)
        context.game.name = game_name

        #if script:
        #    print(repr(script))
        #from .javascript import JavaScript
        #context.game.script = JavaScript(script)

        # FIXME
        context.game.config["name"] = variant_name
        #context.game.platform = platform_name

        for key, value in config.iteritems():
            context.game.config[key] = value

        #if context.game.script:
        #print(context.game.config['name'])
        #print(context.game.config.name)

        #context.game.script.configure(context.game.config)

        #if fs.windows:
        #    p = os.path.join(GameCenter.get_local_data_dir(),
        #            NameUtil.create_cmpname(platform_name),
        #            NameUtil.create_cmpname(game_name), "game.json")
        #    if os.path.exists(p):
        #        with open(p, 'rb') as f:
        #            doc = json.load(f)
        #            if "config" in doc:
        #                for k, v in doc["config"].iteritems():
        #                    context.game.config[k.lower()] = v

        """
        from .gamescanner import GameScanner
        config_file = GameScanner.get_config_file_for_game_file(game_file)
        if config_file:
            cp = ConfigParser.ConfigParser()
            cp.readfp(config_file)
            if cp.has_section('default'):
                for key in cp.options('default'):
                    value = cp.get('default', key)
                    context.game.config[key.lower()] = value
            variant_name = ""
            if ',' in config_name:
                variant_name = config_name.split(',', 1)[0].strip()
            if cp.has_section(variant_name):
                for key in cp.options(variant_name):
                    value = cp.get(variant_name, key)
                    context.game.config[key.lower()] = value
        """

        print("\n" + "-" * 79 + "\n" + "CONFIGURATION")
        for key in context.game.config.keys():
            value = context.game.config[key]
            print("  {0:16s} : {1}".format(key, value))
        print("")

        # FIXME: DEPRECATED
        #context.config.fullscreen = Settings.fullscreen_game
        return context
        """
        controller = GameController.create_for_game_context(context)
        controller.set_options(
                vsync=True,
                stretch=True,
                #filters=['pal'],
                filters=[],
                smooth=True,
                fullscreen=Settings.fullscreen_game
        )
        return controller
        """

