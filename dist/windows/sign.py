import os
import sys
import subprocess

print("a")
if os.path.exists("c:/signtool.py"):
    assert os.system("python c:/signtool.py \"{}\"".format(
        sys.argv[1])) == 0
    # p = subprocess.Popen(["python", "c:/signtool.py", sys.argv[1]])
    # assert p.wait() == 0
