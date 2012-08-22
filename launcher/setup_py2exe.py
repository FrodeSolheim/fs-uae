import os
import glob
from distutils.core import setup
import py2exe

try:
    # py2exe 0.6.4 introduced a replacement modulefinder.
    # This means we have to add package paths there, not to the built-in
    # one.  If this new modulefinder gets integrated into Python, then
    # we might be able to revert this some day.
    # if this doesn't work, try import modulefinder
    try:
        import py2exe.mf as modulefinder
    except ImportError:
        import modulefinder
    import win32com, sys
    for p in win32com.__path__[1:]:
        modulefinder.AddPackagePath("win32com", p)
    for extra in ["win32com.shell"]: #,"win32com.mapi"
        __import__(extra)
        m = sys.modules[extra]
        for p in m.__path__[1:]:
            modulefinder.AddPackagePath(extra, p)
except ImportError:
    # no build path setup, no worries.
    pass

options = {}

py2exe_options = {
    "dll_excludes": ["MSVCP90.dll"],
}
options["py2exe"] = py2exe_options

from py2exe.build_exe import py2exe as build_exe

class LauncherBuildExe(build_exe):

    def copy_extensions(self, extensions):
        """Copy the missing extensions."""
        build_exe.copy_extensions(self, extensions)

        res_dir = os.path.join("fs_uae_launcher", "res")
        full = os.path.join(self.collect_dir, res_dir)
        if not os.path.exists(full):
            self.mkpath(full)

        for f in glob.glob(res_dir + '/*.*'):
            name = os.path.basename(f)
            self.copy_file(f, os.path.join(full, name))
            self.compiled_files.append(os.path.join(res_dir, name))

setup(
name="fs_uae_launcher",
cmdclass = {
    "py2exe": LauncherBuildExe,
},
options=options,
version="1.3.24",
author="Frode Solheim",
author_email = "fs-uae@fengestad.no",
#packages=["fs_uae_launcher"],
#console=["scripts/fs-uae-launcher"],
windows=["scripts/fs-uae-launcher"],
#data_files=[
#    ("fs_uae_launcher/res", [
#        "fs_uae_launcher/res/floppy.png",
#        "fs_uae_launcher/res/joystick.png",
#        "fs_uae_launcher/res/kickstart.png",
#        "fs_uae_launcher/res/model.png",
#    ]),
#]
)
