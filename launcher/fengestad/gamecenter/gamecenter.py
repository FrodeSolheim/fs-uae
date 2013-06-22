from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import sys
import json
from fengestad import fs
from .config import Config


class GameCenter(object):
    game_dirs = []
    data_dir = None
    save_dir = None

    @classmethod
    def init(cls):
        for arg in sys.argv:
            if arg.startswith('--game-dir='):
                cls.game_dirs.append(arg[11:])
        k = 0
        path = fs.get_lib_dir()
        p = os.path.dirname(path)
        while p != path:
            if fs.normalize_path(p) == \
                        fs.normalize_path(fs.get_home_dir()):
                break
            if os.path.exists(os.path.join(p, 'Games')):
                cls.game_dirs.append(os.path.join(p, 'Games'))
                print("found 'local' game dir ", repr(cls.game_dirs[-1]))
                break
            path = p
            p = os.path.dirname(path)
        while p != path:
            if os.path.exists(os.path.join(p, 'Data', 'FSGameCenter')):
                cls.data_dir = os.path.join(p, 'Data', 'FSGameCenter')
                print("found 'local' data dir ", repr(cls.data_dir))
                break
            path = p
            p = os.path.dirname(path)
        while p != path:
            if os.path.exists(os.path.join(p, 'Save')):
                cls.save_dir = os.path.join(p, 'Save')
                print("found 'local' save dir ", repr(cls.save_dir))
                break
            path = p
            p = os.path.dirname(path)
        if len(cls.game_dirs) == 0:
            while True:
                path = Config.get('GameDirectory/%d' % k)
                if not path:
                    break
                cls.game_dirs.append(path.strip())
                k += 1
        if len(cls.game_dirs) == 0:
            path = os.path.join(fs.get_home_dir(), u'Games')
            if os.path.exists(path):
                print("auto add", path)
                cls.game_dirs.append(path)
            path = os.path.join(fs.get_documents_dir(), u'Games')
            if os.path.exists(path):
                print("auto add", path)
                cls.game_dirs.append(path)
            if fs.windows:
                path = "C:\\Games"
                if os.path.exists(path):
                    print("auto add", path)
                    cls.game_dirs.append(path)
        print("game dirs:", cls.game_dirs)

        if not cls.save_dir:
            # FIXME: Use known folder SAVED_GAMES (or something) on Vista+
            cls.save_dir = os.path.join(fs.get_home_dir(), "Games",
                    "Saves")

        if not cls.data_dir:
            cls.data_dir = fs.get_app_data_dir()

    @classmethod
    def get_local_data_dir(cls):
        """local data dir with resources overriding those downloaded from
        the game metadata server."""
        data_dir = fs.get_app_data_dir()
        data_dir = data_dir.replace(fs.get_app_id(), "fs-game-database")
        data_dir = os.path.join(data_dir, "data")
        return data_dir

    @classmethod
    def register_user_activity(cls):
        if fs.windows:
            pass
        elif fs.macosx:
            pass
        else:
            path = '/tmp/fs-game-center-activity'
            if not os.path.exists(path):
                try:
                    with open(path, 'wb') as f:
                        pass
                except Exception:
                    pass
                try:
                    os.chmod(path, 0666)
                except Exception:
                    pass
            os.utime('/tmp/fs-game-center-activity', None)

