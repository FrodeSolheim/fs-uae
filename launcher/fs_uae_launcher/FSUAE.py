from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

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
        proc = subprocess.Popen(args, **kwargs)
        return proc

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
