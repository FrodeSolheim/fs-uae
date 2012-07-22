import sys

class System:
    macosx = (sys.platform == "darwin")
    windows = sys.platform.startswith("win")
    linux = sys.platform.startswith("linux")
