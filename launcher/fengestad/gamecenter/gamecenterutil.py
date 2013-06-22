from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import shutil
import subprocess
import zipfile
from fengestad import fs
import pkg_resources


class GameCenterUtil(object):

    @classmethod
    def get_program_path(cls, name):
        def search(path):
            print("search", path)
            if not os.path.exists(path):
                return None
            if fs.macosx:
                p = os.path.join(path, name + ".app", "Contents",
                        "MacOS", name)
                if os.path.exists(p):
                    return p
            for item in [name] + os.listdir(path):
                dp = os.path.join(path, item)
                if item == name:
                    p = dp
                    if fs.windows:
                        p = p + u".exe"
                    if os.path.exists(p):
                        return p
                if os.path.isdir(dp):
                    p = os.path.join(dp, name)
                    if fs.windows:
                        p = p + u".exe"
                    print("checking", p)
                    if os.path.exists(p):
                        return p

        # try to find emulator in its own project directory
        path = search(os.path.join("..", name, "out"))
        if path and os.path.exists(path):
            return path

        # try to find packaged emulator in a platform workspace dir
        #if fs.windows:
        #    path = search("../fs-game-center-windows/lib")
        #    if path and os.path.exists(path):
        #        return path
        #if fs.macosx:
        #    path = search("../fs-game-center-macosx/lib")
        #    if not path or not os.path.exists(path):
        #        path = search(os.path.expanduser("~/git/fs-game-center-macosx/lib"))
        #    if path and os.path.exists(path):
        #        return os.path.join(os.getcwd(), path)

        # try to find the emulator in game center's lib dir
        #path = search(fs.get_lib_dir())
        #if path and os.path.exists(path):
        #    return path

        # if not on Windows, try to find a system-installed emulator
        if not fs.windows:
            path = "/usr/bin/" + name
            if os.path.exists(path):
                print("checking", path)
                return path
            path = "/usr/local/bin/" + name
            if os.path.exists(path):
                print("checking", path)
                return path

        # or on Mac, search Applications dir for bundle
        if fs.macosx:
            path = search("/Applications")
            if path and os.path.exists(path):
                return path

    @classmethod
    def run_program(cls, name, **kwargs):
        path = cls.get_program_path(name)
        if not path:
            raise Exception("Could not find program {0}".format(name))
        print("program", name, "at", path)
        path = os.path.join(os.getcwd(), path)
        path = os.path.normpath(path)
        print("program path (normalized):", path)
        dir_name = os.path.dirname(path)
        kwargs["env"]["LD_LIBRARY_PATH"] = fs.encode_path(dir_name)
        kwargs["args"].insert(0, path)
        print("run:", kwargs["args"])
        print("cwd:", kwargs.get("cwd", ""))
        return subprocess.Popen(**kwargs)
        #for entry_point in pkg_resources.iter_entry_points(
        #        "fengestad.program", name):
        #    # return first available, load should return a callable
        #    run_func = entry_point.load()
        #    break
        #else:
        #    raise Exception("Could not find program {0}".format(name))
        #print("run program ", name, "with args", kwargs["args"])
        ##print("run program ", name, "with args", kwargs)
        #return run_func(**kwargs)

    @classmethod
    def copy_folder_tree(cls, sourcepath, destpath, overwrite=False):
        for item in os.listdir(sourcepath):
            if item[0] == ".":
                continue
            itempath = os.path.join(sourcepath, item)
            destitempath = os.path.join(destpath, item)
            if os.path.isdir(itempath):
                if not os.path.exists(destitempath):
                    os.makedirs(destitempath)
                cls.copy_folder_tree(itempath, destitempath)
            else:
                if overwrite or not os.path.exists(destitempath):
                    shutil.copy(itempath, destitempath)

    @classmethod
    def unpack(cls, archive, destination):
        if archive.endswith(".7z"):
            cls.unpack_7zip(archive, destination)
        elif archive.endswith(".zip"):
            cls.unpack_zip(archive, destination)
        else:
            raise Exception("do not know how to unpack " + repr(archive))

    @classmethod
    def unpack_zip(cls, archive, destination):
        try:
            print("unpack", archive, "to", destination)
            zip = zipfile.ZipFile(archive, "r")

            def extract_members(zip):
                for name in zip.namelist():
                    if ".." in name:
                        continue
                    if not os.path.normpath(os.path.join(destination, name)
                            ).startswith(destination):
                        raise Exception("Invalid path")
                    yield name
            zip.extractall(path=destination, members=extract_members(zip))
            zip.close()
        except Exception, e:
            print(e)
            raise fs.cause(Exception("Could not unpack game"), e)

    @classmethod
    def unpack_7zip(cls, archive, destination):
        try:
            print("unpack", archive, "to", destination)
            if not os.path.exists(destination):
                os.makedirs(destination)
            #plugin = pyapp.plug.get_plugin("no.fengestad.7zip")
            args = ["x", "-o" + destination, archive]
            process = cls.run_program("7za", args=args,
                                      stdout=subprocess.PIPE)
            #process = plugin.sevenz(args, stdout=subprocess.PIPE)
            line = process.stdout.readline()
            while line:
                print(line[:-1])
                #if fs.windows:
                line = fs.unicode_path(line[:-len(os.linesep)])
                if line.startswith("Extracting  "):
                    parts = line.rsplit(os.sep, 1)
                    if len(parts) == 2:
                        #text = "Extracting " + parts[1]
                        #self.on_status(status="Extracting", sub_status=parts[1])
                        pass
                line = process.stdout.readline()
            returncode = process.wait()
            if returncode != 0:
                raise Exception("7z return non-zero")
        except Exception, e:
            print(e)
            raise fs.cause(Exception("Could not unpack game"), e)
