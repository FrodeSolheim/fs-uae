from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import traceback
import fs_uae_launcher.six as six
from fs_uae_launcher.six.moves import queue

def get_screen_size():
    print("FIXME: get_screen_size")
    return (1920, 1080)

from PySide.QtCore import QEvent, QObject, QCoreApplication
from PySide.QtGui import QErrorMessage, QMessageBox

class CustomEvent(QEvent):
    
    def __init__(self):
        QEvent.__init__(self, QEvent.User)

class EventHandler(QObject):

    def __init__(self):
        QObject.__init__(self)
        self.queue = queue.Queue()

    def customEvent(self, event):
        while True:
            try:
                function, args, kwargs = self.queue.get_nowait()
            except queue.Empty:
                break
            try:
                function(*args, **kwargs)
            except Exception:
                #log.warn("callback event failed: %r %r",
                #        self.callback, self.args, exc_info=True)
                print("-- callback exception --")
                traceback.print_exc()

    def post_callback(self, function, *args, **kwargs):
        self.queue.put((function, args, kwargs))
        QCoreApplication.instance().postEvent(self, CustomEvent())

event_handler = EventHandler()

def call_after(function, *args, **kwargs):
    event_handler.post_callback(function, *args, **kwargs)

def call_later(duration, function, *args, **kwargs):
    print("FIXME: call_later", duration, function)

def show_error(message):
    #QErrorMessage().showMessage(message)
    message_box = QMessageBox()
    message_box.setText(message)
    message_box.exec_()
