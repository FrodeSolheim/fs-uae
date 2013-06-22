from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import threading
import traceback

main_thread_id = threading.current_thread().ident


class Signal():

    signal_listeners = {}
    listeners = {}
    notifications = []
    lock = threading.Lock()

    def __init__(self, signal):
        self.signal = signal

    def connect(self, listener):
        with self.lock:
            Signal.signal_listeners.setdefault(
                self.signal, []).append(listener)
            Signal.listeners.setdefault(listener, []).append(self.signal)

    # def connect(self, listener, *signals):
    #     with self.lock:
    #         for signal in signals:
    #             self.signal_listeners.setdefault(signal, []).append(listener)
    #             self.listeners.setdefault(listener, []).append(signal)

    def disconnect(self, listener):
        with self.lock:
            # print("removing listener", listener)
            Signal.signal_listeners[self.signal].remove(listener)
            Signal.listeners[listener].remove(self.signal)
            if len(Signal.listeners[listener]) == 0:
                del Signal.listeners[listener]

    @classmethod
    def disconnect_all(cls, listener):
        with Signal.lock:
            for signal in Signal.listeners[listener]:
                Signal.signal_listeners[signal].remove(listener)
            del Signal.listeners[listener]

    def notify(self, *args):
        if threading.current_thread().ident == main_thread_id:
            Signal.process_signal(self.signal, *args)

        else:
            with Signal.lock:
                Signal.notifications.append((self.signal, args))
            # FIXME
            import fsui as fsui
            fsui.call_after(Signal.process_all_signals)

    @classmethod
    def process_all_signals(cls):
        with Signal.lock:
            if len(Signal.notifications) == 0:
                return
            notifications = Signal.notifications[:]
            Signal.notifications = []
        #print(notifications)
        for signal, args in notifications:
            cls.process_signal(signal, *args)

    @classmethod
    def process_signal(cls, signal, *args):
        #print(self.listeners.setdefault(signal, []))
        for listener in Signal.signal_listeners.setdefault(signal, []):
            #print(listener, signal, args)
            try:
                if signal == "config":
                    # FIXME: temporary hack for legacy code
                    listener.on_config(*args)
                    continue
                elif signal == "setting":
                    # FIXME: temporary hack for legacy code
                    listener.on_setting(*args)
                    continue
                try:
                    # FIXME: temporary hack for legacy code
                    func = getattr(listener, "on_" + signal + "_signal")
                except AttributeError:
                    pass
                else:
                    func(*args)
                    continue

                listener(*args)
            except Exception:
                traceback.print_exc()


Signal.quit = Signal("quit")
