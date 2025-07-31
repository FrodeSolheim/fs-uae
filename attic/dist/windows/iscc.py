#!/usr/bin/env python3

import os
import sys
import subprocess

program_files = os.getenv("ProgramFiles(x86)", "")
if not program_files:
    program_files = os.getenv("PROGRAMFILES")
iscc = os.path.join(program_files, "Inno Setup 5", "iscc.exe")
if not os.path.exists(iscc):
    iscc = os.path.join("C:", "Program Files (x86)", "Inno Setup 5",
                        "iscc.exe")

p = subprocess.Popen([iscc, sys.argv[1]])
sys.exit(p.wait())
