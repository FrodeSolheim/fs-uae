from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import os
import shutil
import hashlib

class GameChangeHandler(object):

    def __init__(self, path):
        self._preserve_changes_dir = path

    def init(self, state_dir, ignore=[]):
        print("\n" + "-" * 79 + "\n" + "CHANGEHANDLER INIT")
        path = self._preserve_changes_dir
        if os.path.exists(state_dir):
            print("Merging preserved changes in", state_dir, "to", path)
            if state_dir[-1] == "/" or state_dir[-1] == "\\":
                state_dir = state_dir[:-1]
            lstate_dir = len(state_dir)
            first_iteration = True
            for dirpath, dir_names, file_names in os.walk(state_dir):
                for file_name in file_names:
                    ignore_file = False
                    if first_iteration:
                        for ig in ignore:
                            assert ig[0] == "*"
                            dummy, ext = os.path.splitext(file_name)
                            if ext.lower() == ig[1:]:
                                print("ignoring", file_name)
                                ignore_file = True
                                break
                    if ignore_file:
                        continue
                    sourcepath = os.path.join(dirpath, file_name)
                    destpath = os.path.join(path, sourcepath[lstate_dir+1:])
                    if os.path.getsize(sourcepath) == 17:
                        with open(sourcepath, "rb") as f:
                            if f.read() == "FILE_IS_DELETED":
                                print("- removing file",
                                        sourcepath[lstate_dir+1:])
                                if os.path.exists(destpath):
                                    os.remove(destpath)
                                else:
                                    print("  (already gone)")
                                continue
                    print("- updating file", sourcepath[lstate_dir+1:])
                    if not os.path.isdir(os.path.dirname(destpath)):
                        os.makedirs(os.path.dirname(destpath))
                    shutil.copyfile(sourcepath, destpath)
                first_iteration = False
        else:
            print("no game state")
        self._preserve_changes_files = self.create_file_version_list(path)
        print("done")

    def update(self, state_dir):
        print("\n" + "-" * 79 + "\n" + "CHANGEHANDLER UPDATE")
        oldfiles = self._preserve_changes_files
        newfiles = self.create_file_version_list(self._preserve_changes_dir)
        print("checking files")
        for filename, newcs in newfiles.iteritems():
            try:
                oldcs = oldfiles[filename]
            except KeyError:
                print("New file:", filename)
                oldcs = None
            if newcs != oldcs:
                print("File changed:", filename)
                print("-", newcs, "vs", oldcs)
                sourcepath = os.path.join(self._preserve_changes_dir, filename)
                destpath = os.path.join(state_dir,
                        filename)
                print("Writing file", destpath)
                if not os.path.exists(os.path.dirname(destpath)):
                    os.makedirs(os.path.dirname(destpath))
                shutil.copyfile(sourcepath, destpath)

        for filename in oldfiles:
            if not filename in newfiles:
                print("File removed", filename)
                destpath = os.path.join(state_dir, filename)
                if not os.path.exists(os.path.dirname(destpath)):
                    os.makedirs(os.path.dirname(destpath))
                with open(destpath, "wb") as f:
                    f.write("FILE_IS_DELETED")
        print("done")

    def create_file_version_list(self, path):
        print("create_file_version_list")
        if path[-1] == "/" or path[-1] == "\\":
            path = path[:-1]
        lpath = len(path)
        files = {}
        for dirpath, dirnames, filenames in os.walk(path):
            for filename in filenames:
                filepath = os.path.join(dirpath, filename)
                checksum = self.md5file(filepath)
                files[filepath[lpath+1:]] = checksum
        print(" - found %d files (checksummed)" % len(files,))
        return files

    def md5file(self, file):
        with open(file, "rb") as f:
            m = hashlib.md5()
            while True:
                buffer = f.read(4096)
                if buffer == "":
                    break
                m.update(buffer)
            return m.hexdigest()
