from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

import time
import threading
import traceback
import fs_uae_launcher.fsui as fsui
from .Database import Database
from .FileScanner import FileScanner
from .ConfigurationScanner import ConfigurationScanner
from .Config import Config
from .Settings import Settings
from .I18N import _, ngettext

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
            # this must be called from main, since callbacks are broadcast
            # when settings are changed
            Settings.set("last_scan", str(time.time()))
            Settings.set("config_refresh", str(time.time()))
            Config.update_kickstart()
        # call on_done from main thread
        fsui.call_after(on_done)
        cls.running = False

    @classmethod
    def _scan_thread(cls):
        scanner = FileScanner(cls.paths, cls.scan_for_roms, cls.scan_for_files,
                cls.scan_for_configs, on_status=cls.on_status,
                stop_check=cls.stop_check)
        scanner.scan()
        if cls.stop_check():
            return
        if cls.scan_for_configs:
            scanner = ConfigurationScanner(cls.paths, on_status=cls.on_status,
                    stop_check=cls.stop_check)
            scanner.scan()

    @classmethod
    def start(cls, paths, scan_for_roms, scan_for_files, scan_for_configs):
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
        threading.Thread(target=cls.scan_thread).start()

    @classmethod
    def on_status(cls, status):
        cls.status = status
