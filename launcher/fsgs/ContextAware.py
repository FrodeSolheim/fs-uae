from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import weakref

class ContextAware(object):

    def __init__(self, context):
        self.__context = weakref.ref(context)

    @property
    def context(self):
        return self.__context()
