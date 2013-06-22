from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import time
import traceback
#import fsui as fsui

#from fengestad.base.Application import Application
from fengestad.base.Signal import Signal
from fengestad.base.Thread import Thread

#from .FileScanner import FileScanner
#from .ConfigurationScanner import ConfigurationScanner
#from .Config import Config
#from .Settings import Settings

#from .I18N import _, ngettext

#from fsgs.GameDatabase import GameDatabase
#from fsgs.GameDatabaseClient import GameDatabaseClient
#from .OverlayDatabase import OverlayDatabase
#from fsgs.GameDatabaseSynchronizer import GameDatabaseSynchronizer

class ScannerThread(Thread):

    def __init__(self, scanners):
        Thread.__init__(self)
        self.error = ""
        self.scanners = scanners

        #self.running = False
        self.status = ("", "")

    def thread_function(self):
        try:
            self._run()
        except Exception, e:
            self.error = unicode(e)
            traceback.print_exc()

        #def on_done():

            # this must be called from main, since callbacks are broadcast
            # when settings are changed

            #Signal.broadcast_main("scan_done")

            # FIXME
            #Config.update_kickstart()

        # call on_done from main thread
        #fsui.call_after(on_done)
        Signal.broadcast("scan_done")

        #self.running = False

    def _run(self):
        for scanner in self.scanners:
            self.stop_check()
            scanner.stop_check = self.stop_check
            scanner.scan()

        # if cls.update_game_database:
        #     game_database = GameDatabase.get_instance()
        #     game_database_client = GameDatabaseClient(game_database)
        #     synchronizer = GameDatabaseSynchronizer(game_database_client,
        #             on_status=cls.on_status, stop_check=cls.stop_check)
        #     synchronizer.username = Settings.get("database_username")
        #     synchronizer.password = Settings.get("database_password")
        #     synchronizer.synchronize()
        # else:
        #     game_database = None

        # if cls.scan_for_roms or cls.scan_for_files or cls.scan_for_configs:
        #     scanner = FileScanner(cls.paths, cls.scan_for_roms,
        #             cls.scan_for_files, cls.scan_for_configs,
        #             on_status=cls.on_status, stop_check=cls.stop_check)
        #     scanner.scan()
        #     if cls.stop_check():
        #         return

        # database = Database()
        # if cls.scan_for_configs or cls.update_game_database:
        #     scanner = ConfigurationScanner(cls.paths, on_status=cls.on_status,
        #             stop_check=cls.stop_check)
        #     scanner.scan(database, game_database)

    # @classmethod
    # def start(cls, paths, scan_for_roms, scan_for_files, scan_for_configs,
    #         update_game_database=False):
    #     print("Scanner.start")
    #     if cls.running:
    #         print("scan already in progress")
    #         return
    #     cls.paths = paths[:]
    #     cls.running = True
    #     cls.error = ""
    #     cls.stop_flag = False
    #     cls.status = ("", "")
    #     cls.scan_for_roms = scan_for_roms
    #     cls.scan_for_files = scan_for_files
    #     cls.scan_for_configs = scan_for_configs
    #     cls.update_game_database = update_game_database
    #     threading.Thread(target=cls.scan_thread).start()

    # @classmethod
    # def on_status(cls, status):
    #     cls.status = status
