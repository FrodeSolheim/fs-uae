from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

from .ContextAware import ContextAware

class BaseContext(ContextAware):

    def __init__(self, main_context):
        ContextAware.__init__(self, main_context)

    @property
    def config(self):
        return self.context.config

    def set_config(self, *args):
        return self.context.config.set(*args)
