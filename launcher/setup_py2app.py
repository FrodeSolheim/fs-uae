import os
import sys
if "--game-center" in sys.argv:
    app = "FS-UAE Game Center"
    sys.argv.remove("--game-center")
else:
    app = "FS-UAE Launcher"

from setuptools import setup

from py2app.build_app import py2app as py2app_original

if app == "FS-UAE Game Center":

    APP = ["fs-uae-game-center.py"]
    DATA_FILES = []
    OPTIONS = {
        "argv_emulation": True,
        "excludes": "fs_uae_launcher.ui,fsui,wx,wxversion",
    }

    setup(
    name = "FS-UAE Game Center",
    version = "9.8.7",
    author = "Frode Solheim",
    author_email = "fs-uae@fengestad.no",
    app = APP,
    data_files = DATA_FILES,
    options = {'py2app': OPTIONS},
    setup_requires = ['py2app'],
    )

    #os.system("zip -ru dist/\"FS-UAE Game Center.app\"/Contents/Resources/" +
    #    "lib/python2.7/site-packages.zip fs_uae_launcher/res")
    #os.system("zip -ru dist/\"FS-UAE Game Center.app\"/Contents/Resources/" +
    #    "lib/python2.7/site-packages.zip fengestad/gamecenter/res")
    #os.system("zip -ru dist/\"FS-UAE Game Center.app\"/Contents/Resources/" +
    #    "lib/python2.7/site-packages.zip fengestad/input/res")

else:

    APP = ["fs-uae-launcher.py"]
    DATA_FILES = []
    OPTIONS = {
        "argv_emulation": True,
        "excludes": "fengestad,fsui.qt,PySide",
    }

    setup(
    name = "FS-UAE Launcher",
    version = "9.8.7",
    author = "Frode Solheim",
    author_email = "fs-uae@fengestad.no",
    app = APP,
    data_files = DATA_FILES,
    options = {'py2app': OPTIONS},
    setup_requires = ['py2app'],
    )

    os.system("zip -ru dist/\"FS-UAE Launcher.app\"/Contents/Resources/" +
        "lib/python2.7/site-packages.zip fs_uae_launcher/res")
