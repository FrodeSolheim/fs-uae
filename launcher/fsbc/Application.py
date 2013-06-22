from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
from fsbc.Paths import Paths

windows = sys.platform.startswith("win")
macosx = sys.platform.startswith("darwin")


class Application(object):
    
    _instance = None

    @classmethod
    def instance(cls):
        """
        :rtype : Application
        """
        return cls._instance

    @classmethod
    def get_instance(cls):
        """
        :deprecated
        """
        return cls._instance

    @classmethod
    def get(cls):
        """
        :deprecated
        """
        return cls._instance

    @classmethod
    def set_instance(cls, instance):
        """

        :type instance: Application
        """
        if cls._instance:
            raise RuntimeError("An application instance already exists")
            #print("WARNING: An application instance already exists")
        cls._instance = instance

    def __init__(self, name=""):
        #if Application.instance is not None:
        #    raise Exception("An application instance already exists")
        #Application.instance = self
        self.stop_flag = False
        self.name = name
        Application.set_instance(self)

    def __del__(self):
        self.destroy()

    def destroy(self):
        self.stop()
        if self == Application._instance:
            Application._instance = None

    def set_stop_flag(self):
        self.stop_flag = True

    def stop(self):
        self.set_stop_flag()

    def stopping(self):
        return self.stop_flag

    def run_in_main(self, function, *args, **kwargs):
        raise NotImplementedError("Application.run_in_main")

    #def timer(self, timeout, function, *args, **kwargs):
    #    raise NotImplementedError("Application.call_later")

    def executable_dir(self):
        dir_path = os.path.dirname(sys.executable)
        return Paths.unicode(dir_path)

    def data_dirs(self):
        base_dirs = []
        #    "share",
        #]
        if len(sys.argv) > 0:
            script_dir = os.path.dirname(sys.argv[0])
            base_dirs.append(os.path.join(script_dir, "share"))
        if windows:
            base_dirs.append(os.path.join(self.executable_dir(), "share"))
        elif macosx:
            base_dirs.append(os.path.join(self.executable_dir(), "..",
                                          "Resources", "share"))
        else:
            # FIXME: $XDG_DATA_DIRS, $XDG_DATA_HOME
            base_dirs.append("/usr/local/share")
            base_dirs.append("/usr/share")
        data_dirs = []
        for dir in base_dirs:
            data_dir = os.path.join(dir, self.name)
            print("* checking for data dir")
            if os.path.exists(data_dir):
                data_dirs.append(data_dir)
        return data_dirs

    def data_file(self, name):
        """Looks up an application data file in several places depending on
        platform.

        :rtype : str
        """
        for data_dir in self.data_dirs():
            path = os.path.join(data_dir, name)
            print("- checking", path)
            if os.path.exists(path):
                return path
        raise LookupError(name)


def call_after(func, *args, **kwargs):
    return Application.instance().run_in_main(func, *args, **kwargs)
