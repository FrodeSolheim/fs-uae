from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

class IRCBroadcaster:
    irc_listeners = []

    @classmethod
    def add_listener(cls, listener):
        cls.irc_listeners.append(listener)

    @classmethod
    def remove_listener(cls, listener):
        cls.irc_listeners.remove(listener)

    @classmethod
    def broadcast(cls, key, value):
        for listener in cls.irc_listeners:
            listener.on_irc(key, value)
