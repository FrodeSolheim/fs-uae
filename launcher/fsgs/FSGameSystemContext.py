from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import time
import threading
import traceback

from fsgs.Archive import Archive
from .BaseContext import BaseContext
from .Downloader import Downloader
from .FileDatabase import FileDatabase
from .GameDatabase import GameDatabase


class FileContext(BaseContext):

    def __init__(self, main_context):
        BaseContext.__init__(self, main_context)

    def find_by_sha1(self, sha1):
        database = FileDatabase.get_instance()
        result = database.find_file(sha1=sha1)["path"]
        #if not result:
        #    result = self.context.get_game_database().find_file_by_sha1(sha1)
        return result

    def check_sha1(self, sha1):
        database = FileDatabase.get_instance()
        result = database.check_sha1(sha1)
        #if not result:
        #    result = self.context.get_game_database().find_file_by_sha1(sha1)
        return result

    def get_license_code_for_url(self, url):
        return self.context.get_game_database().get_license_code_for_url(url)

    # FIXME: better name
    def convert_uri(self, uri):
        if uri.startswith("sha1://"):
            return self.open_sha1_uri(uri)
        elif uri.startswith("db://"):
            # old name for sha1://
            return self.open_sha1_uri(uri)
        elif uri.startswith("http://"):
            return self.open_url(uri)
        elif uri.startswith("https://"):
            return self.open_url(uri)
        else:
            return Archive(uri).open(uri)

    def open(self, uri):
        while isinstance(uri, basestring):
            uri = self.convert_uri(uri)
        # "uri" should now either be a file object, or None
        assert uri is None or hasattr(uri, "read")
        return uri

    def open_sha1_uri(self, uri):
        sha1 = uri.split("/")[2]
        assert len(sha1) == 40
        return self.find_by_sha1(sha1)

    def open_url(self, url):
        original_url = url
        hash_part = ""
        parts = url.split("#", 1)
        if len(parts) > 1:
            url = parts[0]
            hash_part = "#" + parts[1]
        if not Downloader.cache_file_from_url(url, download=False):
            license_code = self.get_license_code_for_url(original_url)
            license_status = {
                "accepted": False,
                "done": False
            }

            def show_license_code():
                try:
                    try:
                        license_status["accepted"] = self.show_license_code(
                            license_code)
                    except Exception:
                        traceback.print_exc()
                finally:
                    license_status["done"] = True

            if license_code:
                print("URL", url, "has license code", license_code)
                # FIXME: remove direct dependency on fsui
                import fsui as fsui
                fsui.call_after(show_license_code)
                while not license_status["done"]:
                    time.sleep(0.1)
                if not license_status["accepted"]:
                    # FIXME: custom exception here
                    raise Exception("Usage terms \"{0}\" was not "
                                    "accepted".format(license_code))
        path = Downloader.cache_file_from_url(url)
        return path + hash_part

    def show_license_code(self, license_code):
        if license_code == "BTTR":
            license_text = (
                "Files for this game are provided by back2roots.org.\n\n"
                "By using back2roots.org or any of their services you "
                "agree to their Acceptable Usage Policy:\n\n"
                "http://www.back2roots.org/About/Project/Policy/")
        else:
            license_text = license_code
        return self.on_show_license_information(license_text)

    def on_show_license_information(self, license_text):
        print("*** on_show_license_information not implemented ***")
        raise Exception("on_show_license_information not implemented")


class FSGameSystemContext(object):

    def __init__(self):
        self._amiga = None
        self._config = None
        self._signal = None
        self._netplay = None
        self.file = FileContext(self)
        self.thread_local = threading.local()

    @property
    def amiga(self):
        if self._amiga is None:
            from .amiga.AmigaContext import AmigaContext
            self._amiga = AmigaContext(self)
        return self._amiga

    @property
    def config(self):
        if self._config is None:
            from .Config import Config
            self._config = Config(self)
        return self._config

    @property
    def signal(self):
        if self._signal is None:
            from .Signal import Signal
            self._signal = Signal(self)
            #self._signal = Signal()
        return self._signal

    @property
    def netplay(self):
        if self._netplay is None:
            from .netplay.NetplayContext import NetplayContext
            self._netplay = NetplayContext(self)
        return self._netplay

    def get_game_database(self):
        if not hasattr(self.thread_local, "game_database"):
            # FIXME
            from fs_uae_launcher.FSUAEDirectories import FSUAEDirectories
            path = os.path.join(
                FSUAEDirectories.get_data_dir(), "Game Database.sqlite")
            self.thread_local.game_database = GameDatabase(path)
        return self.thread_local.game_database

    @property
    def cache_dir(self):
        # FIXME: remove dependency
        from fs_uae_launcher.FSUAEDirectories import FSUAEDirectories
        return FSUAEDirectories.get_cache_dir()
