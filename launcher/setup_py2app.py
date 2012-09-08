import os
from setuptools import setup

from py2app.build_app import py2app as py2app_original

APP = ["fs-uae-launcher.py"]
DATA_FILES = []
OPTIONS = {'argv_emulation': True}

setup(
name = "FS-UAE Launcher",
version = "1.3.26",
author = "Frode Solheim",
author_email = "fs-uae@fengestad.no",
app = APP,
data_files = DATA_FILES,
options = {'py2app': OPTIONS},
setup_requires = ['py2app'],
)

os.system("zip -ru dist/\"FS-UAE Launcher.app\"/Contents/Resources/" +
        "lib/python2.7/site-packages.zip fs_uae_launcher/res")

