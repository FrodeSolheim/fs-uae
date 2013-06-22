from __future__ import division
from __future__ import with_statement
from __future__ import absolute_import

import os
import ConfigParser
import threading
from fengestad import fs
import pygame
from fengestad.gamecenter.glui.settings import Settings
from fengestad.gamecenter.javascript import JavaScript
from fengestad.gamecenter.gamecenter import GameCenter
from fengestad.gamecenter.database import Database
from fengestad.gamecenter.gamecontext import GameContext

class GameRunner(object):
    
    def __init__(self, controller=None, platform=None, name=None, config=None, **kwargs):
        self.controller = controller
        self.platform_name = platform
        self.game_name = name
        self.config_name = config
        self.done = False
        self.status = ""
        self.error = ""
        self.backtrace = None

    def prepare(self):
        self.done = False
        threading.Thread(target=self._prepare_thread).start()

    def _prepare_thread(self):
        try:
            self._do_prepare()
        except Exception, e:
            import traceback
            traceback.print_exc()
            self.error = repr(e)
            self.backtrace = traceback.format_exc(e)
        self.done = True
        self.signal()

    def _do_prepare(self):
        self.controller.prepare_game()

    def configure(self):
        self.status = ""
        self.error = ""
        self.done = False
        threading.Thread(target=self._configure_thread).start()

    def _configure_thread(self):
        try:
            self._do_configure()
        except Exception, e:
            import traceback
            traceback.print_exc()
            # signal error by setting status on done
            self.error = repr(e)
            self.backtrace = traceback.format_exc(e)
        self.done = True
        self.signal()

    def _do_configure(self):
        self.controller.configure_game()

    def run(self):
        self.status = ""
        self.error = ""
        self.done = False
        threading.Thread(target=self._run_thread).start()

    def abort(self):
        self.controller.abort()

    def _run_thread(self):
        try:
            self._do_run()
        except Exception, e:
            import traceback
            traceback.print_exc()
            self.error = repr(e)
            self.backtrace = traceback.format_exc(e)
        self.done = True
        self.signal()

    def _do_run(self):
        if fs.windows:
            pass
        elif fs.macosx:
            pass
        else:
            # prevent flashing mouse cursor when launching programs 
            # by moving cursor to the bottom right of the screen
            import gtk.gdk
            display = gtk.gdk.display_get_default()
            screen = display.get_default_screen()
            display.warp_pointer(screen, screen.get_width() - 1,
                    screen.get_height() - 1)
        self.controller.run_game()

    def signal(self):
        # create a dummy event to wake up the main thread
        event = pygame.event.Event(pygame.USEREVENT)
        pygame.event.post(event)

