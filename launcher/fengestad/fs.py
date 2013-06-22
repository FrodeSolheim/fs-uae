from __future__ import print_function

import os
import sys
import functools
import ConfigParser


windows = sys.platform == 'win32'
linux = sys.platform.startswith('linux')
macosx = sys.platform == 'darwin'


if windows:
    from win32com.shell import shell, shellcon
    import win32api
else:
    import getpass


def memoize(func):
    memoize_dict = {}
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        try:
            key = (args, frozenset(kwargs.items()))
        except TypeError:
            # cannot create key -- for instance, passing a list as an argument.
            # FIXME: Log warning here
            return func(*args, **kwargs)
        try:
            return memoize_dict[key]
        except KeyError:
            value = func(*args, **kwargs)
            try:
                memoize_dict[key] = value
            except TypeError:
                # uncachable -- for instance, passing a list as an argument.
                # FIXME: Log warning here
                # FIXME: will not happen here.. se above type error?
                pass
            return value
    return wrapper


def cache(func):
    first_time = [True, None]
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        if first_time[0] == True:
            first_time[1] = func(*args, **kwargs)
            first_time[0] = False
        return first_time[1]
    return wrapper


@cache
def get_lib_dir():
    lib_dir = os.environ.get('LIB_DIR', '').decode('UTF-8')
    if lib_dir:
        return unicode_path(lib_dir)
    #os.path.join(os.getcwd, sys.argv[0]
    lib_dir = os.path.join(os.getcwd(), 'lib')
    if os.path.exists(lib_dir):
        return lib_dir
    #raise RuntimeError("could not detect lib dir")
    return ""


EXCEPTION = "EXCEPTION"


@cache
def config_file():
    cp = ConfigParser.ConfigParser()
    path = os.path.join(get_app_config_dir(), get_app_id() + u".cfg")
    print("config file:", path)
    try:
        cp.read([path])
    except Exception, e:
        print(repr(e))
    return cp

    def get_int(self, key, default=None):
        try:
            return int(self.get(key, default))
        except Exception:
            return default

    def get_float(self, key, default=None):
        try:
            return float(self.get(key, default))
        except Exception:
            return default

    def get_bool(self, key, default=None):
        try:
            value = self.get(key, default).strip().lower()
            #print value
            if value in ['1', 'true']:
                return True
            elif value in ['0', 'false']:
                return False
            else:
                return default
        except Exception:
            return default


def config_string(key, default=None):
    section, option = key.lower().split('/', 1)
    section = utf8(section)
    option = utf8(option)
    #with self.lock:
    #    if not self._initialized:
    #        self.initialize()
    #    try:
    print("config", key)
    cp = config_file()
    try:
        value = cp.get(section, option, raw = True)
        return unicode(value , 'utf-8')
    except (ConfigParser.NoSectionError, ConfigParser.NoOptionError):
        return default
    except Exception:
        return default


def config_bool(key, default=None):
    try:
        value = config_string(key, default).strip().lower()
        if value in ['1', 'true']:
            return True
        elif value in ['0', 'false']:
            return False
        else:
            return default
    except Exception:
        return default


def config_int(key, default=None):
    try:
        return int(config_string(key, default))
    except Exception:
        return default


def config_float(key, default=None):
    try:
        return float(config_string(key, default))
    except Exception:
        return default


@cache
def get_app_id():
    # FIXME
    if windows or macosx:
        return "FSGameCenter"
    else:
        return "fs-game-center"


@cache
def get_data_dir():
    if windows:
        path = shell.SHGetFolderPath(0, shellcon.CSIDL_APPDATA, 0, 0)
    elif macosx:
        path = os.path.join(get_home_dir(), 'Library', 'Application Support')
    else:
        path = os.path.join(get_home_dir(), '.local', 'share')
        path = os.environ.get('XDG_DATA_HOME', path)
        path = unicode_path(path)
    if not os.path.exists(path):
        os.makedirs(path)
    return path


@cache
def get_app_data_dir():
    path = os.path.join(get_data_dir(), get_app_id())
    if not os.path.exists(path):
        os.makedirs(path)
    return path


@cache
def get_app_config_dir():
    if windows:
        path = os.path.join(get_app_data_dir(), 'config')
    elif macosx:
        path = os.path.join(get_home_dir(), 'Library', 'Preferences',
                get_app_id())
    else:
        path = os.path.join(get_home_dir(), '.config', get_app_id())
        path = os.environ.get('XDG_CONFIG_HOME', path)
        path = unicode_path(path)
    if not os.path.isdir(path):
        os.makedirs(path)
    return path


@cache
def get_app_cache_dir():
    if windows:
        path = os.path.join(get_data_dir(), 'cache')
    elif macosx:
        path = os.path.join(get_home_dir(), 'Library', 'Caches',
                get_app_id())
    else:
        path = os.path.join(get_home_dir(), '.cache', get_app_id())
        path = os.environ.get('XDG_CACHE_HOME', path)
        path = unicode_path(path)
    if not os.path.isdir(path):
        os.makedirs(path)
    return path


def cause(exc, cause):
    exc.__cause__ = cause
    cause.__traceback__ = sys.exc_info()[2]
    return exc


def encode_path(path):
    from fsbc.Paths import Paths
    return Paths.encode(path)


def unicode_path(path):
    from fsbc.Paths import Paths
    return Paths.unicode(path)


def utf8(object):
    return unicode_safe(object, 'utf-8').encode('utf-8')


def utf8_safe(object):
    return unicode_safe(object, 'utf-8').encode('utf-8')


def unicode_safe(object, encoding = 'ASCII'):
    try:
        return unicode(object)
    except Exception:
        pass
    try:
        return unicode(object, encoding, 'replace')
    except Exception:
        pass
    try:
        return unicode(str(object), encoding, 'replace')
    except Exception:
        #logger.exception("Error in pyapp.unicode_safe")
        return u"String returned from unicode_safe (problem logged)"


def normalize_path(path):
    path = os.path.normcase(os.path.normpath(path))
    return unicode_path(path)


@memoize
def get_user_name():
    if windows:
        # FIXME: This is not tested on Windows yet
        return win32api.GetUserName()
    else:
        return getpass.getuser()


@memoize
def get_desktop_dir(allow_create=True):
    if windows:
        path = shell.SHGetFolderPath(0, shellcon.CSIDL_DESKTOP, 0, 0)
    else:
        #path = pyapp.lib.xdg.user_dir_lookup_with_fallback('DESKTOP',
        #        os.path.join(get_home_dir(), 'Desktop'))
        path = os.path.join(get_home_dir(), 'Desktop')
    path = unicode_path(path)
    if allow_create and not os.path.isdir(path):
        os.makedirs(path)
    return path


@memoize
def get_documents_dir(allow_create=True):
    if windows:
        path = shell.SHGetFolderPath(0, shellcon.CSIDL_PERSONAL, 0, 0)
    else:
        #path = pyapp.lib.xdg.user_dir_lookup_with_fallback('DOCUMENTS',
        #        os.path.join(get_home_dir(), 'Documents'))
        path = os.path.join(get_home_dir(), 'Documents')
    path = unicode_path(path)
    if allow_create and not os.path.isdir(path):
        os.makedirs(path)
    return path


@memoize
def get_pictures_dir(allow_create=True):
    if windows:
        path = shell.SHGetFolderPath(0, shellcon.CSIDL_MYPICTURES, 0, 0)
    else:
        #path = pyapp.lib.xdg.user_dir_lookup_with_fallback('PICTURES',
        #        os.path.join(get_home_dir(), 'Pictures'))
        path = os.path.join(get_home_dir(), 'Pictures')
    path = unicode_path(path)
    if allow_create and not os.path.isdir(path):
        os.makedirs(path)
    return path


@memoize
def get_home_dir():
    if windows:
        path = shell.SHGetFolderPath(0, shellcon.CSIDL_PROFILE, 0, 0)
        return path
    return unicode_path(os.path.expanduser("~"))
