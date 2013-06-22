from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import functools
import subprocess
from .System import System

if System.is_windows:
    from win32com.shell import shell, shellcon
    import win32api
else:
    import getpass

# FIXME: move to util
def cache(func):
    first_time = [True, None]
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        if first_time[0] == True:
            first_time[1] = func(*args, **kwargs)
            first_time[0] = False
        return first_time[1]
    return wrapper

# FIXME: move to util
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


class Paths:

    @staticmethod
    def encode_path(path):
        from fsbc.Paths import Paths
        return Paths.encode(path)

    @staticmethod
    def unicode_path(path):
        from fsbc.Paths import Paths
        return Paths.unicode(path)

    @classmethod
    @cache
    def get_data_dir(cls):
        if System.is_windows:
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_APPDATA, 0, 0)
        elif System.is_macosx:
            path = os.path.join(cls.get_home_dir(), "Library",
                    "Application Support")
        else:
            path = os.path.join(cls.get_home_dir(), ".local", "share")
            path = os.environ.get("XDG_DATA_HOME", path)
            path = cls.unicode_path(path)
        return path

    @classmethod
    @cache
    def get_cache_dir(cls):
        if System.is_windows:
            path = cls.get_data_dir()
        elif System.is_macosx:
            path = os.path.join(cls.get_home_dir(), "Library", "Caches")
        else:
            path = os.path.join(cls.get_home_dir(), ".cache")
            path = os.environ.get("XDG_CACHE_HOME", path)
            path = cls.unicode_path(path)
        return path

    @classmethod
    @cache
    def get_config_dir(cls):
        if System.is_windows:
            path = cls.get_data_dir()
        elif System.is_macosx:
            path = os.path.join(cls.get_home_dir(), "Library", "Preferences")
        else:
            path = os.path.join(cls.get_home_dir(), ".config")
            path = os.environ.get("XDG_CONFIG_HOME", path)
            path = cls.unicode_path(path)
        return path

    @classmethod
    @memoize
    def get_desktop_dir(cls, allow_create=True):
        if System.is_windows:
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_DESKTOP, 0, 0)
        else:
            path = xdg_user_dir("DESKTOP")
            if not path:
                path = os.path.join(cls.get_home_dir(), "Desktop")
        path = cls.unicode_path(path)
        if allow_create and not os.path.isdir(path):
            os.makedirs(path)
        return path

    @classmethod
    @memoize
    def get_documents_dir(cls, allow_create=True):
        if System.is_windows:
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_PERSONAL, 0, 0)
        elif System.is_macosx:
            path = os.path.join(cls.get_home_dir(), "Documents")
        else:
            path = xdg_user_dir("DOCUMENTS")
            if not path:
                path = cls.get_home_dir()
        path = cls.unicode_path(path)
        if allow_create and not os.path.isdir(path):
            os.makedirs(path)
        return path

    @classmethod
    @memoize
    def get_pictures_dir(cls, allow_create=True):
        if System.is_windows:
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_MYPICTURES, 0, 0)
        else:
            path = xdg_user_dir("PICTURES")
            if not path:
                path = os.path.join(cls.get_home_dir(), "Pictures")
        path = cls.unicode_path(path)
        if allow_create and not os.path.isdir(path):
            os.makedirs(path)
        return path

    @classmethod
    @memoize
    def get_home_dir(cls):
        if System.is_windows:
            path = shell.SHGetFolderPath(0, shellcon.CSIDL_PROFILE, 0, 0)
            return path
        return cls.unicode_path(os.path.expanduser("~"))

@memoize
def xdg_user_dir(name):
    if System.is_windows or System.is_macosx:
        return None
    try:
        proc = subprocess.Popen(["xdg-user-dir", name],
                stdout=subprocess.PIPE)
        path = proc.stdout.read().strip()
        path = path.decode("UTF-8")
        print("XDG user dir {0} => {1}".format(name, repr(path)))
    except Exception:
        path = None
    print("xdg_user_dir", name, "path is", path)
    return path
