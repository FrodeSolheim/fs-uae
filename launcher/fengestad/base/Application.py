from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
from .Paths import Paths
from fsbc.Application import Application as BaseApplication


class Application(BaseApplication):

    instance = None
    name = None

    #@classmethod
    #def get(cls, name=None):
    #    #if name is not None:
    #    #    return cls(name)
    #    #if cls.instance is None:
    #    #    cls.instance = cls(cls.name)
    #    #return cls.instance
    #    return cls.instance()

    @classmethod
    def init(cls, name):
        cls.name = name

    def __init__(self, name):
        BaseApplication.__init__(self, name)
        assert name
        #self.name = name
        #self.stop_flag = False

    #def set_stop_flag(self):
    #    self.stop_flag = True

    #def stop(self):
    #    self.set_stop_flag()

    def get_config_dir(self):
        path = os.path.join(Paths.get_config_dir(), self.name)
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    def get_data_dir(self):
        path = os.path.join(Paths.get_data_dir(), self.name)
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    def get_cache_dir(self):
        path = os.path.join(Paths.get_cache_dir(), self.name)
        if not os.path.exists(path):
            os.makedirs(path)
        return path

    def get_config_path(self, name):
        return os.path.join(self.get_config_dir(), name)

    def get_data_path(self, name):
        return os.path.join(self.get_data_dir(), name)

    def get_cache_path(self, name):
        return os.path.join(self.get_cache_dir(), name)

    def is_stopping(self):
        return self.stop_flag
