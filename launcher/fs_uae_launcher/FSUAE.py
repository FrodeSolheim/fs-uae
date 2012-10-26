from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import tempfile
import subprocess
import fs_uae_launcher.fs as fs

class FSUAE:

    @classmethod
    def start_with_config(cls, config):
        print("FSUAE.start_with_config:")
        tf = tempfile.NamedTemporaryFile(suffix=".fs-uae", delete=False)
        config_file = tf.name
        with tf:
            for line in config:
                print(line)
                tf.write(line.encode("UTF-8"))
                tf.write("\n")
        args = [config_file]
        return cls.start_with_args(args), config_file

    @classmethod
    def start_with_args(cls, args, **kwargs):
        print("FSUAE.start_with_args:", args)
        exe = cls.find_executable()
        print("current dir (cwd): ", os.getcwd())
        print("using fs-uae executable:", exe)
        args = [exe] + args
        print(args)
        env = os.environ.copy()
        #env = None
        cls.center_window(env)
        #print(repr(env))
        proc = subprocess.Popen(args, env=env, **kwargs)
        return proc

    @classmethod
    def center_window(cls, env):
        from .Config import Config
        from .Settings import Settings
        width = Config.get("window_width") or Settings.get("window_width")
        height = Config.get("window_height") or Settings.get("window_height")
        try:
            width = int(width)
        except:
            width = 960
        try:
            height = int(height)
        except:
            height = 540
        from .ui.MainWindow import MainWindow
        if MainWindow.instance is None:
            return

        main_w, main_h = MainWindow.instance.get_size()
        main_x, main_y = MainWindow.instance.get_position()

        x = main_x + (main_w - width) // 2
        y = main_y + (main_h - height) // 2
        if fs.windows:
            import wx
            y += wx.SystemSettings_GetMetric(wx.SYS_CAPTION_Y)
        env[str("SDL_VIDEO_WINDOW_POS")] = str("{0},{1}".format(x, y))
        #print("window position", env["SDL_VIDEO_WINDOW_POS"])
        #os.environ["SDL_VIDEO_WINDOW_POS"] = "{0},{1}".format(x, y)

    @classmethod
    def find_executable(cls):
        if fs.windows:
            if os.path.exists("out/fs-uae.exe"):
                # for testing / development
                return "out/fs-uae.exe"
        else:
            if os.path.exists("out/fs-uae"):
                # for testing / development
                return "out/fs-uae"
        if fs.windows:
            exe = "../FS-UAE.exe"
        elif fs.macosx:
            exe = "../../../FS-UAE.app/Contents/MacOS/fs-uae"
        else:
            exe = "fs-uae"
        return exe
