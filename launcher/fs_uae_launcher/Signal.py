from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

class Signal:

    listeners = {}

    @classmethod
    def add_listener(cls, signal, listener):
        cls.listeners.setdefault(signal, []).append(listener)

    @classmethod
    def remove_listener(cls, signal, listener):
        cls.listeners[signal].remove(listener)

    @classmethod
    def broadcast(cls, signal, *args):
        for listener in cls.listeners.setdefault(signal, []):
            if signal == "config":
                listener.on_config(*args)
            elif signal == "setting":
                listener.on_setting(*args)
            else:
                getattr(listener, "on_" + signal + "_signal")(*args)
