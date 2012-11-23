from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import time
import threading
import traceback
import fs_uae_launcher.fsui as fsui
from .Database import Database
from .FileScanner import FileScanner
from .ConfigurationScanner import ConfigurationScanner
from .Config import Config
from .Settings import Settings
from .Signal import Signal
from .I18N import _, ngettext

from .fsgs.GameDatabase import GameDatabase
from .fsgs.GameDatabaseClient import GameDatabaseClient
from .OverlayDatabase import OverlayDatabase
from .fsgs.GameDatabaseSynchronizer import GameDatabaseSynchronizer

class Scanner:
    #listeners = []

    #@classmethod
    #def add_listener(cls, listener):
    #    cls.listeners.append(listener)

    #@classmethod
    #def remove_listener(cls, listener):
    #    cls.listeners.append(listener)

    running = False
    stop_flag = False
    status = ("", "")

    @classmethod
    def stop_check(cls):
        return cls.stop_flag

    @classmethod
    def scan_thread(cls):
        try:
            cls._scan_thread()
        except Exception:
            traceback.print_exc()

        def on_done():
            # FIXME: these should be removed soon
            Settings.set("last_scan", str(time.time()))
            Settings.set("config_refresh", str(time.time()))

            # this must be called from main, since callbacks are broadcast
            # when settings are changed

            Signal.broadcast("scan_done")
            Config.update_kickstart()
        # call on_done from main thread
        fsui.call_after(on_done)
        cls.running = False

    @classmethod
    def _scan_thread(cls):
        if cls.update_game_database:
            game_database = GameDatabase.get_instance()

            game_database_client = GameDatabaseClient(game_database)
            synchronizer = GameDatabaseSynchronizer(game_database_client,
                    on_status=cls.on_status, stop_check=cls.stop_check)
            synchronizer.synchronize()
        else:
            game_database = None

        if cls.scan_for_roms or cls.scan_for_files or cls.scan_for_configs:
            scanner = FileScanner(cls.paths, cls.scan_for_roms,
                    cls.scan_for_files, cls.scan_for_configs,
                    on_status=cls.on_status, stop_check=cls.stop_check)
            scanner.scan()
            if cls.stop_check():
                return

        database = Database()
        if cls.scan_for_configs or cls.update_game_database:
            scanner = ConfigurationScanner(cls.paths, on_status=cls.on_status,
                    stop_check=cls.stop_check)
            scanner.scan(database, game_database)

    @classmethod
    def start(cls, paths, scan_for_roms, scan_for_files, scan_for_configs,
            update_game_database=False):
        print("Scanner.start")
        if cls.running:
            print("scan already in progress")
            return
        cls.paths = paths[:]
        cls.running = True
        cls.stop_flag = False
        cls.status = ("", "")
        cls.scan_for_roms = scan_for_roms
        cls.scan_for_files = scan_for_files
        cls.scan_for_configs = scan_for_configs
        cls.update_game_database = update_game_database
        threading.Thread(target=cls.scan_thread).start()

    @classmethod
    def on_status(cls, status):
        cls.status = status
