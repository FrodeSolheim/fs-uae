from __future__ import division
from __future__ import print_function
from __future__ import absolute_import


import os
import sys
import time
import json
import traceback
import threading
from collections import deque
from .database import Database
from fengestad.gamecenter.gamecontext import GameContext
from fengestad.gamecenter.GameHandler import GameHandler


class MainQueueItem:
    done = False
    exception = None
    result = None

_main_thread = threading.currentThread()

class Main:

    queue = deque()
    lock = threading.Lock()

    @classmethod
    def process(cls):
        with cls.lock:
            for i in range(len(cls.queue)):
                item = cls.queue.popleft()
                try:
                    item.result = item.func(*item.args, **item.kwargs)
                except Exception, e:
                    import traceback
                    traceback.print_exc()
                    item.exception = e
                item.done = True

    @classmethod
    def call(cls, func, *args, **kwargs):
        if threading.currentThread() == _main_thread:
            return func(*args, **kwargs)
        item = MainQueueItem()
        item.func = func
        item.args = args
        item.kwargs = kwargs
        with cls.lock:
            cls.queue.append(item)
        while not item.done:
            time.sleep(0.01)
        if item.exception:
            raise item.exception
        return item.result


def main():
    #import logging; logging.shutdown(); import sys; sys.exit()
    print("Fengestad Game System...")

    from .Application import Application
    #application = \
    Application("fs-uae-game-center")

    from fengestad.gamecenter.gamecenter import GameCenter
    GameCenter.init()
    #metadata_file = os.path.join(game_dir, 'Metadata.json')
    #if not os.path.exists(metadata_file):
    #    print("no game found")
    #    sys.exit(1)
    #print("using game dir", game_dir)
    #with open(metadata_file, 'rb') as f:
    #    metadata = json.load(f)
    #print(metadata)
    #configurations_file = os.path.join(game_dir, 'Configurations.json')
    #if os.path.exists(configurations_file):
    #    with open(configurations_file, 'rb') as f:
    #        configurations = json.load(f)['configurations']
    #else:
    #    configurations = [{}]
    #print(configurations)

    #gamedata_dir = os.path.join(game_dir, 'Gamedata')

    
    #resources = {}
    #base_path = os.path.expanduser('~/Resources')
    #for item in os.listdir(base_path):
    #    if item.endswith('(Resource)'):
    #        name = item[:-10].strip()
    #        resources[name] = os.path.join(base_path, item)

    for arg in sys.argv[:]:
        remove_arg = True
        if arg.startswith('--database='):
            Database.path = arg[11:]
        #elif os.path.exists(arg):
        #    game_file = arg
        else:
            remove_arg = False
        if remove_arg:
            sys.argv.remove(arg)


    #context = GameContext(metadata, gamedata_dir, resources=resources)
    for arg in sys.argv[1:]:
        if os.path.exists(arg):
            game_file = arg
            print(game_file)
            break
    else:
        for arg in sys.argv[1:]:
            if not arg.startswith('--') and not arg.startswith('-psn'):
                break
        else:
            Database.open()
            if '--scan' in sys.argv:
                from .gamescanner import GameScanner
                GameScanner.scan()
                sys.exit(0)
            #plugin = pyapp.plugins.get_plugin('no.fengestad.gamecenter.glui')
            #plugin.main()
            
            class OutWrapper:
                def __init__(self, f):
                    self.f = f
                def write(self, msg):
                    self.f.write(msg)
                    #self.f.write("(")
                    #self.f.write(repr(traceback.format_stack()))
                    self.f.write(traceback.format_stack()[-2])
            #sys.stdout = OutWrapper(sys.stdout)
            
            import fengestad.gamecenter.glui
            fengestad.gamecenter.glui.main()
            print(" --- fengestad.gamecenter.glui.main is done ---")
            return
        print("usage: fs-game-center [gamefile]")
        print("(args was " + repr(sys.argv) + ")")
        sys.exit(1)
            #remove_arg = True
            #if arg == '--fullscreen':
            #    pass
            #elif arg == '--no-fullscreen':
            #    pass
            #elif arg == '--vsync':
            #    pass
            #elif arg == '--no-vsync':
            #    pass
            #elif arg.startswith('--database='):
            #    context.database = arg[11:]
            #    #context.database = arg[11:]
            #elif os.path.exists(arg):
            #    game_file = arg
            #else:
            #    remove_arg = False
            #if remove_arg:
            #    sys.argv.remove(arg)
    context = GameContext(game_file)
    """
    for arg in sys.argv[:]:
        remove_arg = True
        if arg == '--fullscreen':
            context.config.fullscreen = True
        elif arg == '--no-fullscreen':
            context.config.fullscreen = False
        elif arg == '--install' or arg == '-i':
            context.config.install = True
        elif arg == '--setup':
            context.config.setup = True
        elif arg == '--vsync':
            context.config.vsync = True
        elif arg == '--no-vsync':
            context.config.vsync = False
        elif arg.startswith('--database='):
            context.database = arg[11:]
        #elif os.path.exists(arg):
        #    game_file = arg
        else:
            remove_arg = False
        if remove_arg:
            sys.argv.remove(arg)
        """
    #print("args:", repr(pyapp.argv))

    if context.database:
        import sqlite3
        connection = sqlite3.connect(context.database)
        path = os.path.normpath(os.path.normcase(game_file))
        cursor = connection.cursor()
        cursor.execute("SELECT Game.id, Game.name, configuration "
                "FROM File, GameFile, Game "
                "WHERE File.id = GameFile.file AND "
                "GameFile.game = Game.id AND File.path = ?",
                (path,))
        row = cursor.fetchone()
        if row:
            game_id, game_name, config_name = row
            #context.game.name = game_name
            #context.game.config_name = config_name
            context.game.id = game_id
            
            cursor.execute("SELECT key, value FROM Data WHERE "
                    "game = ?", (game_id,))
            for row in cursor:
                key, value = row
                #try:
                #    v = context.game.config[key]
                #except KeyError:
                context.game.config[key] = value

    #configuration = configurations[0]
    handler = GameHandler.create_for_game_context(context)
    #done = [False]
    #def on_exit(**kwargs):
    #    print("exit signal received")
    #    done[0] = True
    #controller.on_exit.connect(on_exit)
    #controller.run_game(configuration)
    configuration = {}
    handler.prepare_game()
    handler.run_game(configuration)
    #while not done[0]:
    #    time.sleep(0.1)

