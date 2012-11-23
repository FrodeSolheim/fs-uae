from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import fs_uae_launcher.fs as fs
from .Settings import Settings

class Paths:

    @classmethod
    def join(cls, a, b):
        #if not a:
        #    return b
        #if a[-1] == "/" or a[-1] == "\\":
        #    return a + b
        #return a + "/" + b
        return os.path.join(a, b).replace("\\", "/")

    @classmethod
    def expand_path(cls, path, default_dir=None):
        if path and path[0] == "$":
            cmp_path = path.upper().replace("\\", "/")
            if cmp_path.startswith("$BASE/"):
                return cls.join(cls.get_base_dir(), path[6:])
            if cmp_path.startswith("$CONFIG/"):
                config_path = Settings.get("config_path")
                if config_path:
                    return cls.join(os.path.dirname(config_path),
                        path[8:])
            if cmp_path.startswith("$HOME/"):
                return cls.join(cls.get_home_dir(), path[6:])
            # FIXME: $base_dir is deprecated
            if cmp_path.startswith("$BASE_DIR/"):
                return cls.join(Settings.get_base_dir(), path[10:])
        elif not os.path.isabs(path) and default_dir is not None:
            return os.path.join(default_dir, path)
        return path

    @classmethod
    def contract_path(cls, path, default_dir=None):
        print("before", path)
        path = cls.get_real_case(path)
        print("after", path)
        #dir, file = os.path.split(path)
        #norm_dir = dir + "/"
        if default_dir is not None:
            default_dir = default_dir + "/"
            if path.startswith(default_dir):
                return path[len(default_dir):]
        base_dir = cls.get_base_dir(slash=True)
        if path.startswith(base_dir):
            path = path.replace(base_dir, "$BASE/")
        home_dir = cls.get_home_dir(slash=True)
        if path.startswith(home_dir):
            path = path.replace(home_dir, "$HOME/")
        return path

    @classmethod
    @fs.memoize
    def get_base_dir(cls, slash=False):
        path = Settings.get_base_dir()
        if slash:
            path = path + "/"
        return path

    @classmethod
    @fs.memoize
    def get_home_dir(cls, slash=False):
        path = fs.get_home_dir()
        path = cls.get_real_case(path)
        if slash:
            path = path + "/"
        return path

    @classmethod
    def get_real_case(cls, path):
        """Check the case for the (case insensitive) path. Used to make the
        database portable across sensitive/insensitive file systems."""

        # not really needed on Linux

        parts = []
        drive, p = os.path.splitdrive(path)
        if path.startswith("/"):
            drive = "/"
        elif drive:
            # on Windows, add / to make drive a valid path
            drive = drive + "/"
        last = ""
        while p != last:
            name = os.path.basename(p)#.lower()
            if not name:
                break
            #parts.append(os.path.basename(p).lower())
            parts.append(name)
            last = p
            p = os.path.dirname(p)
        parts.reverse()
        #print(drive, parts)
        result = [drive]
        result.extend(parts)

        combined = drive
        combined = combined.upper()
        k = 1
        for part in parts:
            #print("part is", part)
            if os.path.isdir(combined):
                #print("checking case of", combined+ "/" + part)
                for name in os.listdir(combined):
                    if name.lower() == part.lower():
                        #print("found case =", name)
                        combined += "/" + name
                        result[k] = name
                        break
                else:
                    raise Exception("could not find case for path " + path)
            k += 1
        # normalizing slashes to forward slash to make the database more
        # portable
        result_path = os.path.join(*result).replace("\\", "/")
        return result_path
