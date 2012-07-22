from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

from .IRCHandler import IRCHandler

class LobbyHandler(IRCHandler):

    handler = None

    @classmethod
    def get(cls):
        if cls.handler is None:
            cls.handler = LobbyHandler()
        return cls.handler
