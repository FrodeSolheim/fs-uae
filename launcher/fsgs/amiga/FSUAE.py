from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import tempfile
import traceback
import subprocess
import fs_uae_launcher.fs as fs
from fsbc.Application import Application

try:
    getcwd = os.getcwdu
except AttributeError:
    getcwd = os.getcwd

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
                tf.write(b"\n")
        args = [config_file]
        return cls.start_with_args(args), config_file

    @classmethod
    def start_with_args(cls, args, **kwargs):
        print("FSUAE.start_with_args:", args)
        exe = cls.find_executable()
        print("current dir (cwd): ", getcwd())
        print("using fs-uae executable:", exe)
        args = [exe] + args
        print(args)
        env = os.environ.copy()
        #env = None
        try:
            cls.center_window(env)
        except Exception:
            traceback.print_exc()
        #print(repr(env))
        proc = subprocess.Popen(args, env=env, **kwargs)
        return proc

    @classmethod
    def center_window(cls, env):
        # FIXME: does not really belong here (dependency loop)
        from fs_uae_launcher.Config import Config
        from fs_uae_launcher.Settings import Settings
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
        from fs_uae_launcher.ui.MainWindow import MainWindow
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
        application = Application.instance()
        if os.path.isdir("launcher"):
            # may be running FS-UAE Launcher from source directory, we
            # then want to run the locally compiled fs-uae binary, if any
            for path in ["out/fs-uae", "./fs-uae"]:
                if fs.windows:
                    path += ".exe"
                if os.path.isfile(path):
                    return path
        if os.path.isdir("fs_uae_launcher"):
            # may be running FS-UAE Launcher from source directory, we
            # then want to run the locally compiled fs-uae binary, if any
            for path in ["../out/fs-uae", "../fs-uae"]:
                if fs.windows:
                    path += ".exe"
                if os.path.isfile(path):
                    return path
        if fs.windows:
            exe = os.path.join(
                application.executable_dir(), "Emulator/FS-UAE.exe")
            if not os.path.exists(exe):
                exe = os.path.join(
                    application.executable_dir(), "../Emulator/FS-UAE.exe")
        elif fs.macosx:
            exe = os.path.join(
                application.executable_dir(),
                "../Resources/FS-UAE.app/Contents/MacOS/fs-uae")
            if not os.path.exists(exe):
                exe = os.path.join(
                    application.executable_dir(),
                    "../../../FS-UAE.app/Contents/MacOS/fs-uae")
            if not os.path.exists(exe):
                exe = os.path.join(
                    application.executable_dir(),
                    "../../../Programs/Mac OS X/FS-UAE.app/Contents/MacOS/fs-uae")
            if not os.path.exists(exe):
                exe = os.path.join(
                    application.executable_dir(),
                    "../../../FS-UAE Launcher.app/Contents/Resources/"
                    "FS-UAE.app/Contents/MacOS/fs-uae")
        else:
            return "fs-uae"
        if not os.path.exists(exe):
            raise Exception("Could not find FS-UAE executable")
        return exe
