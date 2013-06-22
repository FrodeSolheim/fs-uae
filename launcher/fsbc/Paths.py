from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import sys


class Paths(object):

    @staticmethod
    def encode(path):
        return path.encode(sys.getfilesystemencoding())

    @staticmethod
    def unicode(path):
        if isinstance(path, unicode):
            return path
        return path.decode(sys.getfilesystemencoding())
