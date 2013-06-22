from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import sys

class System:

    is_windows = sys.platform == "win32"
    is_linux = sys.platform.startswith("linux")
    is_macosx = sys.platform == "darwin"
