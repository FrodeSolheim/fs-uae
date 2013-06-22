from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
import fs_uae_launcher.fs as fs
from .Util import memoize, get_real_case


class FSUAEDirectories:

    base_dir = ""

    @classmethod
    def initialize(cls):
        for arg in sys.argv[1:]:
            arg = arg.decode(sys.getfilesystemencoding())
            if arg.startswith("--"):
                if "=" in arg:
                    key, value = arg[2:].split("=", 1)
                    key = key.replace("-", "_")
                    if key == "base_dir":
                        value = os.path.abspath(value)
                        print("setting base_dir")
                        FSUAEDirectories.base_dir = value
        if FSUAEDirectories.base_dir:
            print(
                "base_dir was specified so we will not check for "
                "portable dir")
        else:
            cls.setup_portable_launcher()
        if not FSUAEDirectories.base_dir:
            print("base_dir not decided yet, checking FS_UAE_BASE_DIR")
            if "FS_UAE_BASE_DIR" in os.environ and \
                    os.environ["FS_UAE_BASE_DIR"]:
                print("base dir specified by FS_UAE_BASE_DIR")
                FSUAEDirectories.base_dir = os.environ["FS_UAE_BASE_DIR"]
        if not FSUAEDirectories.base_dir:
            print("using default base_dir")

    @classmethod
    def setup_portable_launcher(cls):
        """This ab
        ba

        :return: None
        """
        path = os.path.dirname(os.path.abspath(sys.executable))
        last = ""
        while not last == path:
            portable_ini_path = os.path.join(path, "Portable.ini")
            print("checking", portable_ini_path)
            if os.path.exists(portable_ini_path):
                print("detected portable dir", path)
                FSUAEDirectories.base_dir = path
                return
            last = path
            path = os.path.dirname(path)
        print("no Portable.ini found in search path")

    @classmethod
    def read_custom_path(cls, name):
        for app in ["fs-uae-launcher", "fs-uae"]:
            key_path = os.path.join(fs.get_app_config_dir(app), name)
            print("- checking", key_path)
            if os.path.exists(key_path):
                try:
                    with open(key_path, "rb") as f:
                        path = f.read().strip()
                        break
                except Exception as e:
                    print("error reading custom path", repr(e))
        else:
            return None
        lpath = path.lower()
        if lpath.startswith("$home/") or lpath.startswith("$home\\"):
            path = os.path.join(fs.get_home_dir(), path[6:])
        return path

    @classmethod
    def get_base_dir(cls):
        path = cls.base_dir
        if path:
            return path
        return cls._get_base_dir()

    @classmethod
    @memoize
    def _get_base_dir(cls):
        path = cls.read_custom_path("base-dir")
        if not path:
            path = os.path.join(fs.get_documents_dir(True), "FS-UAE")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        print("base dir is", path)
        return path

    @classmethod
    def get_default_search_path(cls):
        paths = []
        path = cls.get_base_dir()
        if fs.windows:
            path = path.replace("/", "\\")
        paths.append(path)

        if fs.windows:
            from win32com.shell import shell, shellcon
            path = shell.SHGetFolderPath(
                0, shellcon.CSIDL_COMMON_DOCUMENTS, 0, 0)
            path = fs.unicode_path(path)
            path = os.path.join(path, "Amiga Files")
            if os.path.exists(path):
                paths.append(path)
        return paths

    @classmethod
    def get_whdload_dir(cls):
        path = os.path.join(cls.get_hard_drives_dir(), "WHDLoad")
        if os.path.exists(path):
            path = get_real_case(path)
            return path
        return None

    @classmethod
    def get_configurations_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Configurations")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_controllers_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Controllers")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_kickstarts_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Kickstarts")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_floppies_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Floppies")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_hard_drives_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Hard Drives")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_cdroms_dir(cls):
        path = os.path.join(cls.get_base_dir(), "CD-ROMs")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_logs_dir(cls):
        path = os.path.join(cls.get_cache_dir(), "Logs")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_data_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Data")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_launcher_dir(cls):
        return cls.get_data_dir()

    @classmethod
    def get_titles_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Titles")
        return path

    @classmethod
    def get_save_states_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Save States")
        if not os.path.exists(path):
            os.makedirs(path)
        path = get_real_case(path)
        return path

    @classmethod
    def get_screenshots_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Screenshots")
        return path

    @classmethod
    def get_images_dir(cls):
        path = cls.get_base_dir()
        return path

    @classmethod
    def get_covers_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Covers")
        return path

    @classmethod
    def get_themes_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Themes")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_plugins_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Plugins")
        if not os.path.exists(path):
            return None
            #os.makedirs(path)
        return path

    @classmethod
    def get_cache_dir(cls):
        path = os.path.join(cls.get_base_dir(), "Cache")
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    @classmethod
    def get_files_dirs(cls):
        paths = [
            cls.get_floppies_dir(),
            cls.get_cdroms_dir(),
            os.path.expanduser("~/Games/Amiga/Files/"),
            os.path.expanduser("~/Games/CD32/Files"),
            os.path.expanduser("~/Games/CDTV/Files")
        ]
        return paths

    @classmethod
    def get_titles_dirs(cls):
        paths = [
            cls.get_titles_dir(),
            os.path.join(fs.get_home_dir(), "Games", "Amiga", "Titles"),
            os.path.join(fs.get_home_dir(), "Games", "CD32", "Titles"),
            os.path.join(fs.get_home_dir(), "Games", "CDTV", "Titles"),
        ]
        return paths

    @classmethod
    def get_screenshots_dirs(cls):
        paths = [
            cls.get_screenshots_dir(),
            os.path.join(fs.get_home_dir(), "Games", "Amiga", "Screenshots"),
            os.path.join(fs.get_home_dir(), "Games", "CD32", "Screenshots"),
            os.path.join(fs.get_home_dir(), "Games", "CDTV", "Screenshots"),
        ]
        return paths

    @classmethod
    def get_images_dirs(cls):
        paths = [
            cls.get_images_dir(),
            os.path.join(fs.get_home_dir(), "Games"),
            #os.path.join(fs.get_home_dir(), "Games",
            #        "Amiga", "Images"),
            #os.path.join(fs.get_home_dir(), "Games",
            #        "CD32", "Images"),
            #os.path.join(fs.get_home_dir(), "Games",
            #        "CDTV", "Images"),
        ]
        return paths

    @classmethod
    def get_covers_dirs(cls):
        paths = [
            cls.get_covers_dir(),
            os.path.join(fs.get_home_dir(), "Games", "Amiga", "Covers"),
            os.path.join(fs.get_home_dir(), "Games", "CD32", "Covers"),
            os.path.join(fs.get_home_dir(), "Games", "CDTV", "Covers"),
        ]
        return paths

    @classmethod
    def get_themes_dirs(cls):
        paths = [cls.get_themes_dir()]
        return paths

    @classmethod
    def get_configurations_dirs(cls):
        paths = [
            cls.get_configurations_dir(),
            #os.path.expanduser("~/Games/Amiga/Configs"),
            os.path.expanduser("~/Games/Amiga/Data"),
            #os.path.expanduser("~/Games/CD32/Configs"),
            os.path.expanduser("~/Games/CD32/Data"),
            #os.path.expanduser("~/Games/CDTV/Configs"),
            os.path.expanduser("~/Games/CDTV/Data")
        ]
        return paths
