from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import sys


class System:
    macosx = (sys.platform == "darwin")
    windows = sys.platform.startswith("win")
    linux = sys.platform.startswith("linux")
