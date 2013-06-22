from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import threading
import traceback
from .ContextAware import ContextAware


class Signal(ContextAware):

    def __init__(self, context):
        ContextAware.__init__(self, context)
        self.signal_listeners = {}
        self.listeners = {}
        self.notifications = []
        self.lock = threading.Lock()

    def connect(self, listener, *signals):
        with self.lock:
            for signal in signals:
                self.signal_listeners.setdefault(signal, []).append(listener)
                self.listeners.setdefault(listener, []).append(signal)

    def disconnect(self, *args):
        if len(args) == 1:
            listener, = args
            with self.lock:
                for signal in self.listeners[listener]:
                    self.signal_listeners[signal].remove(listener)
                del self.listeners[listener]
        elif len(args) == 2:
            signal, listener = args
            with self.lock:
                self.signal_listeners[signal].remove(listener)
        else:
            raise Exception("Invalid number of args to disconnect")

    def notify(self, signal, value):
        with self.lock:
            self.notifications.append((signal, value))
        # FIXME
        import fsui as fsui
        fsui.call_after(self.process)

    def process(self):
        with self.lock:
            if len(self.notifications) == 0:
                return
            notifications = self.notifications[:]
            self.notifications = []
        #print(notifications)
        for signal, value in notifications:
            #print(self.listeners.setdefault(signal, []))
            for listener in self.signal_listeners.setdefault(signal, []):
                print(listener, signal, value)
                try:
                    listener(signal, value)
                except Exception:
                    traceback.print_exc()
