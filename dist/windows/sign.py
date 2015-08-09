import os
import sys
import subprocess

if os.path.exists("c:/signtool.py"):
    assert os.system("python c:/signtool.py \"{}\"".format(
        sys.argv[1])) == 0
