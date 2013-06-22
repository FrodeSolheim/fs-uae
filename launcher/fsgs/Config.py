from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .ContextAware import ContextAware

class Config(ContextAware):

    def __init__(self, context):
        ContextAware.__init__(self, context)

    def get(self, key, default=""):
        from fs_uae_launcher.Config import Config
        return Config.get(key, default)

    def set(self, *values):
        # temporary weird circular package/module interdependencies right now
        # due to restructuring 
        from fs_uae_launcher.Config import Config
        if len(values) == 1:
            Config.set_multiple(*values)
        elif len(values) == 2:
            Config.set(*values)
        else:
            raise Exception("Invalid number of parameters to set_config")
