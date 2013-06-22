from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import threading
from fsgs.GameDatabase import GameDatabase


class OverlayDatabase(GameDatabase):

    thread_local = threading.local()
    database_path = None

    @classmethod
    def get_database_path(cls):
        if not cls.database_path:
            raise Exception("GameDatabase: Database path not set")
        return cls.database_path

    @classmethod
    def set_database_path(cls, path):
        cls.database_path = path

    @classmethod
    def get_instance(cls):
        if not hasattr(cls.thread_local, "instance"):
            cls.thread_local.instance = cls(cls.get_database_path())
        return cls.thread_local.instance
